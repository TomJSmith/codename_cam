#include "Physics.h"

#include <stdexcept>
#include <iostream>

#include "Shader.h"
#include "Transform.h"

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

	flags = PxPairFlag::eCONTACT_DEFAULT;

	return PxFilterFlags();
}

Physics::Physics()
{
	static PxDefaultErrorCallback err;
	static PxDefaultAllocator alloc;

	foundation_ = PxCreateFoundation(PX_PHYSICS_VERSION, alloc, err);
	if (!foundation_) throw std::runtime_error("failed to create physics foundation");

	physics_ = PxCreatePhysics(PX_PHYSICS_VERSION, *foundation_, PxTolerancesScale());
	if (!physics_) throw std::runtime_error("failed to create physics");

	PxSceneDesc scenedesc(physics_->getTolerancesScale());
	scenedesc.gravity = PxVec3(0.0f, -9.8f, 0.0f);
	scenedesc.cpuDispatcher = PxDefaultCpuDispatcherCreate(1);
	scenedesc.filterShader = VehicleFilterShader;
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

	scene_->setVisualizationParameter(PxVisualizationParameter::eSCALE, 1.0f);
	scene_->setVisualizationParameter(PxVisualizationParameter::eCOLLISION_SHAPES, 1.0f);
#endif // #ifdef DEBUG
}

Physics::~Physics()
{
	// We have to free in the opposite order of creation
#ifdef DEBUG
	//Caueses errors without the debug in main working
//	pvd_->release();
#endif

	PxCloseVehicleSDK();

	cooking_->release();
	scene_->release();
	physics_->release();
	foundation_->release();
}

void Physics::Update(seconds dt)
{
	if (dt.count() == 0) return;

	scene_->simulate(dt.count());
	scene_->fetchResults(true);

	PxU32 ntransforms;
	const auto transforms = scene_->getActiveTransforms(ntransforms);

	for (PxU32 i = 0; i < ntransforms; ++i) {
		auto transform = static_cast<Transform *>(transforms[i].userData);
		auto pxtransform = transforms[i].actor2World;
		transform->rotation = quaternion(pxtransform.q.w, pxtransform.q.x, pxtransform.q.y, pxtransform.q.z);
		transform->position = vec3(pxtransform.p.x, pxtransform.p.y, pxtransform.p.z);
	}
}

#ifdef DEBUG
std::vector<Renderer::MeshData> Physics::GetDebugMeshData()
{
	static Shader &shader = Shader::Load("passthrough.vert", "passthrough.frag");

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
			shader.Program(),
			2,
			GL_LINES,
			mat4(1.0)
		});
	}

	return ret;
}
#endif // #ifdef DEBUG