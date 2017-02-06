#include "RigidBody.h"

#include "Entity.h"

using namespace physx;

void RigidBody::RegisterHandlers()
{
	auto t = body_->getGlobalPose();
	entity_->GetTransform().position = vec3(t.p.x, t.p.y, t.p.z);
	entity_->GetTransform().rotation = quaternion(t.q.w, t.q.x, t.q.y, t.q.z);
	body_->userData = &entity_->GetTransform();
}

RigidBody::RigidBody(Physics &physics, PxMaterial &material, PxGeometry &geometry, PxTransform &transform, bool dynamic)
{
	if (dynamic) body_ = physics.GetPhysics()->createRigidDynamic(transform);
	else body_ = physics.GetPhysics()->createRigidStatic(transform);

	auto shape = body_->createShape(geometry, material);

	shape->setQueryFilterData(PxFilterData { 0, 0, 0, Physics::DRIVABLE_SURFACE });
	shape->setSimulationFilterData(PxFilterData { Physics::GROUND, Physics::GROUND_COLLISIONS, 0, 0 });
	physics.GetScene()->addActor(*body_);
}

RigidBody::~RigidBody()
{
	body_->release();
}
