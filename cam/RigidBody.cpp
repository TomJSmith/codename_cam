#include "RigidBody.h"

#include "Entity.h"

using namespace physx;

void RigidBody::RegisterHandlers()
{
	body_->userData = &entity_->GetTransform();
}

RigidBody::RigidBody(Physics &physics, PxMaterial &material, PxGeometry &geometry, PxTransform &transform)
{
	body_ = physics.GetPhysics()->createRigidDynamic(transform);
	body_->createShape(geometry, material);
	physics.GetScene()->addActor(*body_);
}

RigidBody::~RigidBody()
{
	body_->release();
}
