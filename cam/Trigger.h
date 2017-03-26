#pragma once

#include "Physics.h"

#include "Component.h"
#include "Events.h"

class Trigger : public Component {
public:
	// TODO remove transform from this and from rigidbody
	// set the transform from the entity
	Trigger(Physics &physics, PxGeometry &geometry, PxTransform &transform);
	~Trigger();

private:
	virtual void RegisterHandlers() override;

	Physics &physics_;
	PxRigidDynamic *actor_;
};