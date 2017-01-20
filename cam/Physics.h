#pragma once

#include "Time.h"
#include <PhysX/PxPhysicsAPI.h>

using namespace physx;

class Physics
{
public:
	Physics();
	~Physics();

	void Update(seconds dt);

	PxPhysics *GetPhysics() { return physics_; }
	PxScene *GetScene() { return scene_; }
	PxFoundation *GetFoundation() { return foundation_; }

private:
	PxFoundation *foundation_;
	PxPhysics *physics_;
	PxScene *scene_;
};

