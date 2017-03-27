#include "Physics.h"

#include <algorithm>
#include <iostream>
#include <PhysX/PxSimulationEventCallback.h>
#include <stdexcept>

#include "Entity.h"
#include "ModelShader.h"
#include "Transform.h"
#include "Vehicle.h"

const float Physics::Timestep = 1.0f / 500.0f;

class SimulationCallback : public PxSimulationEventCallback {
	void onConstraintBreak(PxConstraintInfo *, PxU32) override {}
	void onWake(PxActor **, PxU32) override {}
	void onSleep(PxActor **, PxU32) override {}
	void onTrigger(PxTriggerPair *pairs, PxU32 count) override {
		for (PxU32 i = 0; i < count; ++i) {
			if (!pairs[i].flags.isSet(PxTriggerPairFlag::eDELETED_SHAPE_OTHER)
				&& !pairs[i].flags.isSet(PxTriggerPairFlag::eREMOVED_SHAPE_OTHER)
				&& !pairs[i].flags.isSet(PxTriggerPairFlag::eDELETED_SHAPE_TRIGGER)
				&& !pairs[i].flags.isSet(PxTriggerPairFlag::eREMOVED_SHAPE_TRIGGER))
			{
				auto t = static_cast<Entity *>(pairs[i].triggerActor->userData);
				auto e = static_cast<Entity *>(pairs[i].otherActor->userData);

				if (pairs[i].status == PxPairFlag::eNOTIFY_TOUCH_FOUND) {
					t->FireEvent(Events::TriggerEnter{ e });
					e->FireEvent(Events::TriggerEnter{ t });
				}
				else {
					t->FireEvent(Events::TriggerExit{ e });
					e->FireEvent(Events::TriggerExit{ t });
				}
			}
		}
	}

	void onContact(const PxContactPairHeader &header, const PxContactPair *pairs, PxU32 n) override {
		if (!header.flags.isSet(PxContactPairHeaderFlag::eREMOVED_ACTOR_0)
			&& !header.flags.isSet(PxContactPairHeaderFlag::eDELETED_ACTOR_0)
			&& !header.flags.isSet(PxContactPairHeaderFlag::eDELETED_ACTOR_1)
			&& !header.flags.isSet(PxContactPairHeaderFlag::eREMOVED_ACTOR_1))
		{
			auto e0 = static_cast<Entity *>(header.actors[0]->userData);
			auto e1 = static_cast<Entity *>(header.actors[1]->userData);

			e0->FireEvent(Events::Collided{ e1 });
			e1->FireEvent(Events::Collided{ e0 });
		}
		else {
			std::cout << "deleted object collision...\n";
		}
	}
};

static PxFilterFlags VehicleFilterShader(PxFilterObjectAttributes /* unused */,
										 PxFilterData data0,
										 PxFilterObjectAttributes /* unused */,
										 PxFilterData data1,
										 PxPairFlags& flags,
										 const void * /* unused */,
										 PxU32 /* unused */)
{
	if (((data0.word0 & data1.word1) == 0) && ((data1.word0 & data0.word1) == 0)) {
		return PxFilterFlag::eSUPPRESS;
	}

	if (data0.word0 & Physics::TRIGGER || (data1.word0 & Physics::TRIGGER)) {
		flags = PxPairFlag::eTRIGGER_DEFAULT;
		return PxFilterFlags();
	}

	flags = PxPairFlag::eCONTACT_DEFAULT | PxPairFlag::eNOTIFY_TOUCH_FOUND;

	return PxFilterFlags();
}

Physics::Physics()
{
	static PxDefaultErrorCallback err;
	static PxDefaultAllocator alloc;
	static SimulationCallback callback;

	foundation_ = PxCreateFoundation(PX_PHYSICS_VERSION, alloc, err);
	if (!foundation_) throw std::runtime_error("failed to create physics foundation");

	physics_ = PxCreatePhysics(PX_PHYSICS_VERSION, *foundation_, PxTolerancesScale());
	if (!physics_) throw std::runtime_error("failed to create physics");

	PxInitExtensions(*physics_);

	PxSceneDesc scenedesc(physics_->getTolerancesScale());
	scenedesc.gravity = PxVec3(0.0f, -9.8f, 0.0f);
	scenedesc.cpuDispatcher = PxDefaultCpuDispatcherCreate(1);
	scenedesc.filterShader = VehicleFilterShader;
	scenedesc.simulationEventCallback = &callback;
	scenedesc.flags |= PxSceneFlag::eENABLE_ACTIVETRANSFORMS;

	scene_ = physics_->createScene(scenedesc);

	if (!scene_) throw std::runtime_error("failed to create physics scene");

	cooking_ = PxCreateCooking(PX_PHYSICS_VERSION, *foundation_, PxCookingParams(PxTolerancesScale()));
	if (!cooking_) throw std::runtime_error("failed to create physics cooking");

	PxInitVehicleSDK(*physics_);
	PxVehicleSetBasisVectors(PxVec3(0.0f, 1.0f, 0.0f), PxVec3(0.0f, 0.0f, 1.0f));

#ifdef DEBUG
	auto pvdmanager = physics_->getPvdConnectionManager();
	if (pvdmanager) {
		pvd_ = PxVisualDebuggerExt::createConnection(pvdmanager, "127.0.0.1", 5425, 1000, PxVisualDebuggerExt::getAllConnectionFlags());
	};

	PxVec3 min(-0.5f, -0.5f, -0.5f);
	PxVec3 max(0.5f, 0.5f, 0.5f);
	scene_->setVisualizationCullingBox(PxBounds3(min, max));
	scene_->setVisualizationParameter(PxVisualizationParameter::eSCALE, 1.0f);
	scene_->setVisualizationParameter(PxVisualizationParameter::eCOLLISION_AABBS, 1.0f);
#endif // #ifdef DEBUG
}

Physics::~Physics()
{
	// We have to free in the opposite order of creation
#ifdef DEBUG
	if (pvd_) pvd_->release();
#endif

	PxCloseVehicleSDK();

	cooking_->release();
	scene_->release();

	PxCloseExtensions();

	physics_->release();
	foundation_->release();
}

void Physics::Update(seconds dt)
{
	if (dt.count() == 0) return;
	static float accumulator = 0.0f;
	accumulator += std::min(dt.count(), 0.1f);

	while (accumulator > Timestep) {
		scene_->simulate(Timestep);
		scene_->fetchResults(true);

		PxU32 ntransforms;
		const auto transforms = scene_->getActiveTransforms(ntransforms);

		for (PxU32 i = 0; i < ntransforms; ++i) {
			auto entity = static_cast<Entity *>(transforms[i].userData);
			if (!entity->GetParent()) {
				// TODO is this actually a problem or should we silently continue here?
				// std::cout << "hmmm...\n";
				continue;
			}
			auto parentpos = entity->GetParent()->GetGlobalPosition();
			auto parentrot = entity->GetParent()->GetGlobalRotation();
			auto &transform = entity->GetTransform();
			auto &pxtransform = transforms[i].actor2World;

			transform.rotation = glm::inverse(parentrot) * quaternion(pxtransform.q.w, pxtransform.q.x, pxtransform.q.y, pxtransform.q.z);
			transform.position = vec3(pxtransform.p.x, pxtransform.p.y, pxtransform.p.z) - parentpos;
		}

		accumulator -= Timestep;

		for (auto vehicle : vehicles_) {
			vehicle->Step(Timestep);
		}
	}
}

void Physics::UnregisterVehicle(Vehicle *vehicle)
{
	vehicles_.erase(std::remove(vehicles_.begin(), vehicles_.end(), vehicle), vehicles_.end());
}

#ifdef DEBUG
std::vector<Renderer::MeshData> Physics::GetDebugMeshData()
{
	static ModelShader shader;

	static std::vector<GLuint> vertexVbos;
	static std::vector<GLuint> colourVbos;
	static std::vector<Renderer::MeshData> ret;

	glDeleteBuffers((GLsizei)vertexVbos.size(), vertexVbos.data());
	glDeleteBuffers((GLsizei)colourVbos.size(), colourVbos.data());

	vertexVbos.clear();
	colourVbos.clear();

	for (auto &i : ret) {
		glDeleteVertexArrays(1, &i.vao);
	}

	ret.clear();

	// TODO there are points, triangles and text we could render too...
	// probably the exact same way as this
	const auto &buffer = scene_->getRenderBuffer();
	auto lines = buffer.getLines();

	for (PxU32 i = 0; i < buffer.getNbLines(); ++i) {
		PxVec3 p0 = lines[i].pos0;
		PxVec3 p1 = lines[i].pos1;
		GLfloat c0 = lines[i].color0;
		GLfloat c1 = lines[i].color1;

		GLfloat vertices[][3] = {
			{p0.x, p0.y, p0.z},
			{p1.x, p1.y, p1.z}
		};

		GLfloat colours[][3] = {
			{c0, c0, c0},
			{c1, c1, c1} // TODO these should be properly transformed to rgb
		};

		GLuint vertexBuffer = 0;
		glGenBuffers(1, &vertexBuffer);
		glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
		glBufferData(GL_ARRAY_BUFFER, 2 * sizeof(GLfloat) * 3, vertices, GL_STATIC_DRAW);

		GLuint colourBuffer = 0;
		glGenBuffers(1, &colourBuffer);
		glBindBuffer(GL_ARRAY_BUFFER, colourBuffer);
		glBufferData(GL_ARRAY_BUFFER, 2 * sizeof(GLfloat) * 3, colours, GL_STATIC_DRAW);

		GLuint vao;
		glGenVertexArrays(1, &vao);
		glBindVertexArray(vao);

		glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
		glEnableVertexAttribArray(0);

		glBindBuffer(GL_ARRAY_BUFFER, colourBuffer);
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, 0);
		glEnableVertexAttribArray(1);

		vertexVbos.push_back(vertexBuffer);
		colourVbos.push_back(colourBuffer);

		ret.push_back(Renderer::MeshData{
			vao,
			2,
			GL_LINES,
			mat4(1.0),
			&shader
		});
	}

	return ret;
}
#endif // #ifdef DEBUG