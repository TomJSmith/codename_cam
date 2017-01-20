#include "Physics.h"

#include <stdexcept>
#include <iostream>

#include "Transform.h"

Physics::Physics()
{
	static PxDefaultErrorCallback err;
	static PxDefaultAllocator alloc;

	foundation_ = PxCreateFoundation(PX_PHYSICS_VERSION, alloc, err);
	if (!foundation_) throw std::runtime_error("failed to create physics foundation");

	physics_ = PxCreatePhysics(PX_PHYSICS_VERSION, *foundation_, PxTolerancesScale());
	if (!physics_) throw std::runtime_error("failed to create physics");

	PxSceneDesc scenedesc(physics_->getTolerancesScale());
	scenedesc.gravity = PxVec3(0.0f, 9.8f, 0.0f);
	scenedesc.cpuDispatcher = PxDefaultCpuDispatcherCreate(1);
	scenedesc.filterShader = PxDefaultSimulationFilterShader;
	scenedesc.flags |= PxSceneFlag::eENABLE_ACTIVETRANSFORMS;

	scene_ = physics_->createScene(scenedesc);
	if (!scene_) throw std::runtime_error("failed to create physics scene");
}

Physics::~Physics()
{
	// We have to free in the opposite order of creation
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