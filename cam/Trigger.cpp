#include "Trigger.h"

#include "Entity.h"

Trigger::Trigger(Physics &physics, PxGeometry &geometry, PxTransform &transform)
	: physics_(physics)
{
	actor_ = PxCreateDynamic(*physics.GetPhysics(), transform, geometry, *physics.GetPhysics()->createMaterial(0.0f, 0.0f, 0.0f), 1.0f);

	PxShape *shape;
	actor_->getShapes(&shape, 1);
	shape->setFlag(PxShapeFlag::eSIMULATION_SHAPE, false);
	shape->setFlag(PxShapeFlag::eTRIGGER_SHAPE, true);
	shape->setSimulationFilterData(PxFilterData { Physics::TRIGGER, Physics::TRIGGER_COLLISIONS, 0, 0 });
	shape->setQueryFilterData(PxFilterData{});
	actor_->is<PxRigidBody>()->setRigidBodyFlag(PxRigidBodyFlag::eKINEMATIC, true);
}

Trigger::~Trigger()
{
	physics_.GetScene()->removeActor(*actor_);
	actor_->release();
}

void Trigger::RegisterHandlers()
{
	actor_->userData = entity_;
	vec3 p = entity_->GetGlobalPosition();
	actor_->setGlobalPose(PxTransform(PxVec3(p.x, p.y, p.z)));

	physics_.GetScene()->addActor(*actor_);
}