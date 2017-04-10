#pragma once

#include <memory>
#include <vector>

#include <PhysX/PxBatchQueryDesc.h>
#include <PhysX/PxPhysicsAPI.h>

#include "windows.h"
#include "Xinput.h"

#include "Events.h"
#include "Component.h"

#include "Controller.h"
#include "aiController.h"

using namespace physx;
class Physics;

class Vehicle : public Component
{
public:
	struct Configuration {
		// Starting position of the vehicle
		PxVec3 position = PxVec3(0.0f, 10.0f, -10.0f);
		PxQuat rotation = PxQuat(PxIdentity);

		// Wheel parameters
		PxF32 wheelMass = 20.0f;
		PxF32 wheelRadius = 0.5f;
		PxF32 wheelWidth = 0.4f;
		PxU32 nWheels = 6;

		// Not certain we really want to be setting this tbh, we might want to just
		// derive it from the wheel mass and radius?...
		PxF32 wheelMOI = 0.5f * 20.0f * 5.0f * 5.0f; //Wheel mass was 20f

		// Chassis parameters
		PxVec3 chassisOffset = PxVec3(0.0f, -1.0f + 0.65f, 0.25f);
		PxVec3 chassisDimensions = PxVec3(2.5f, 2.0f, 5.0f);
		PxF32 chassisMass = 1500.0f;

		// Not certain we actually want to configure this either
		PxVec3 chassisMOI = PxVec3((2.0f * 2.0f + 5.0f * 5.0f) * 1500.0f / 12.0f,
								   (2.5f * 2.5f + 5.0f * 5.0f) * .8f * 1500.0f / 12.0f,
								   (2.5f * 2.5f + 2.0f * 2.0f) * 155.0f / 12.0f);

		// Steering parameters
		PxF32 handbrakeTorque = 4000.0f;
		PxF32 steerAngle = PxPi * 0.3333f;

		// Suspension parameters
		PxF32 maxCompression = 0.3f;
		PxF32 maxDroop = 0.1f;
		PxF32 suspensionStrength = 35000.0f;
		PxF32 suspensionDamping = 4500.0f;

		// Not certain what camber even is but you can totally change it!
		// Has something to do with the angle of the wheels at different suspension travel
		// distances?
		PxF32 camberAtRest = 0.0f;
		PxF32 camberAtMaxDroop = 0.1f;
		PxF32 camberAtMaxCompression = -0.1f;

		// Engine parameters
		PxF32 torque = 3000.0f;
		PxF32 maxSpeed = 1.0f;

		// Also not certain what this is, something engine-related
		PxF32 maxOmega = 1000.0f; //INCREASE THIS TO INCREASE OUR SPEED I BELIEVE

		// Not meant to be set externally - just something that needs to be passed around
		// internally within Vehicle()
		std::vector<PxVec3> wheelOffsets;
	};

	Vehicle(Physics &physics, std::shared_ptr<Controller> controller, Configuration &config = Configuration());

	// For python's sake
	// It can't make shared_ptr<Controller>s out of AiControllers so this ctor converts for us
	Vehicle(Physics &physics, std::shared_ptr<aiController> aicontroller, Configuration &config = Configuration());

	~Vehicle();
	
	void Step(float dt);
	void Update(seconds dt) override;
	PxRigidDynamic *GetActor() { return actor_; }
	Physics &GetPhysics() { return physics_; } // TODO we probably shouldn't need this....

	void SetActive(bool active) { active_ = active; }
	void SetFriction(float friction);

	void getSource(Events::Sound event);


protected:
	void RegisterHandlers() override;

private:
	void Drive();

	Physics &physics_;
	std::shared_ptr<Controller> controller_;

	PxBatchQuery *batchquery_;

	std::vector<PxRaycastQueryResult> querybuffer_;
	std::vector<PxRaycastHit> hitbuffer_;

	PxRigidDynamic *actor_;
	PxVehicleDrive4W *vehicle_;
	PxVehicleDrive4WRawInputData input_;
	PxVehicleDrivableSurfaceToTireFrictionPairs *frictionpairs_;
	bool active_;
	float mass_;

	bool changeSound = false;
	int prevSoundChoice;
	ALuint source;
	int soundChoice;
	int prevChoice;
	vec3 soundPosition;
	std::function<void(Events::Sound)> handler_;
};

