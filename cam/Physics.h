#pragma once

#include <vector>

#include <PhysX/PxPhysicsAPI.h>

#include "Renderer.h"
#include "Time.h"
#include "Vehicle.h"

// This using should be fine because everything in physx:: is prefixed
// with Px anyways, so there will be no name conflicts with anything
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
	PxCooking *GetCooking() { return cooking_; }

	void RegisterVehicle(Vehicle *vehicle) { vehicles_.push_back(vehicle); }

	enum SurfaceFlags : PxU32 {
		DRIVABLE_SURFACE = (1 << 0),
		NONDRIVABLE_SURFACE = (1 << 1)
	};

	enum CollisionFlags : PxU32 {
		WHEEL = (1 << 0),
		GROUND = (1 << 1),
		CHASSIS = (1 << 2),
		TRIGGER = (1 << 3)
	};

	enum CollidesWithFlags : PxU32 {
		WHEEL_COLLISIONS = CollisionFlags::CHASSIS | CollisionFlags::WHEEL, // wheels don't hit the ground, raycasts do that
		GROUND_COLLISIONS = CollisionFlags::CHASSIS | CollisionFlags::GROUND, // ground doesn't hit wheels either
		CHASSIS_COLLISIONS = CollisionFlags::GROUND | CollisionFlags::WHEEL | CollisionFlags::CHASSIS, // chassis hits everything!
		TRIGGER_COLLISIONS = 0xFFFFFFFF, // triggers *literally* hit everything
	};

	static const float Timestep;

private:
	PxFoundation *foundation_;
	PxPhysics *physics_;
	PxScene *scene_;
	PxCooking *cooking_;

	std::vector<Vehicle *> vehicles_;
#ifdef DEBUG
public:
	std::vector<Renderer::MeshData> GetDebugMeshData();

private:
	PxVisualDebuggerConnection *pvd_;

#endif // #ifdef DEBUG
};

