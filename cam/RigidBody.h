#pragma once

#include <PhysX/PxPhysicsAPI.h>
#include <PhysX/geometry/PxGeometry.h>

#include "Component.h"
#include "Physics.h"

class RigidBody : public Component
{
public:
	RigidBody(Physics &physics, physx::PxMaterial &material, physx::PxGeometry &geometry, physx::PxTransform &transform);
	~RigidBody();

	void RegisterHandlers() override;

private:
	physx::PxRigidDynamic *body_;
};

