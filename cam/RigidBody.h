#pragma once

#include <PhysX/PxPhysicsAPI.h>
#include <PhysX/geometry/PxGeometry.h>

#include "Component.h"
#include "Physics.h"

class RigidBody : public Component
{
public:
	RigidBody(Physics &physics, PxMaterial &material, PxGeometry &geometry, PxTransform &transform, bool dynamic = true);
	RigidBody(Physics &physics, PxMaterial &material, const char *objfile, float scale);

	~RigidBody();

	void RegisterHandlers() override;

private:
	PxRigidActor *body_;
	Physics &physics_;
};

