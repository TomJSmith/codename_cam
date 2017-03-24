#pragma once

#include <PhysX/PxPhysicsAPI.h>
#include <PhysX/geometry/PxGeometry.h>

#include "Component.h"
#include "Physics.h"

class RigidBody : public Component
{
public:
	RigidBody(Physics &physics, PxMaterial &material, PxGeometry &geometry, PxTransform &transform, bool dynamic = true);
	RigidBody(Physics &physics, PxMaterial &material, const char *objfile, float scale, bool dynamic = true);

	RigidBody(Physics &physics, const char *objfile, float scale, bool dynamic = true)
		: RigidBody(physics, *physics.GetPhysics()->createMaterial(1.0f, 1.0f, 1.0f), objfile, scale, dynamic) {}

	~RigidBody();

	void RegisterHandlers() override;
	PxRigidActor *GetRigidBody() { return body_; }

private:
	PxRigidActor *body_;
	Physics &physics_;
};