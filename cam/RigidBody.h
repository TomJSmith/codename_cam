#pragma once

#include <PhysX/PxPhysicsAPI.h>
#include <PhysX/geometry/PxGeometry.h>

#include "Component.h"
#include "Physics.h"

class RigidBody : public Component
{
public:
	RigidBody(Physics &physics, PxMaterial &material, PxGeometry &geometry, PxTransform &transform);
	~RigidBody();

	void RegisterHandlers() override;

private:
	PxRigidDynamic *body_;
};

