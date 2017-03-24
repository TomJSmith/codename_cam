#include "Camera.h"

#include <glm/gtx/transform.hpp>
#include <iostream>

#include "Entity.h"
#include "Physics.h"
#include "RigidBody.h"

void Camera::RegisterHandlers()
{
	auto c = Entity::Create(entity_).lock();

	// Save the created child object as a weak_ptr, since we don't own it
	// and it might be destroyed while this camera still exists
	std::weak_ptr<Entity> child = c;

	handler_ = [this, child] (Events::Render e) {
		// Check that the child still exists
		if (auto c = child.lock()) {
			// If so, set the viewing position of the camera to the child, and
			// the rotation to point back at the vehicle
			auto pos = glm::translate(c->GetGlobalPosition());
			auto rot = glm::toMat4(entity_->GetGlobalRotation() * glm::angleAxis(PxPi, vec3(0.0f, 1.0f, 0.0f)));

			e.cameraTransform = pos * rot;
		}
	};

	entity_->GetEvents().RegisterEventHandler(&handler_);

	vec3 offset(0, 6, -25);
	auto pos = entity_->GetGlobalPosition() + offset;

	if (auto t = target_.lock()) {
		auto &phys = t->GetPhysics();

		std::shared_ptr<RigidBody> body(new RigidBody(
			phys,
			*phys.GetPhysics()->createMaterial(0.0f, 0.0f, 0.0f),
			PxSphereGeometry(3.0f),
			PxTransform(PxVec3(pos.x, pos.y, pos.z))
		));

		c->AddComponent(body);

		auto rb = body->GetRigidBody();
		auto targetrb = t->GetActor();
		rb->is<PxRigidDynamic>()->setMass(0.001f);

		auto d6 = PxD6JointCreate(
			*phys.GetPhysics(),
			targetrb,
			PxTransform(PxVec3(0.0f, 0.0f, 0.0f)),
			rb,
			PxTransform(-PxVec3(offset.x, offset.y, offset.z))
		);
	}
}

Camera::Camera(std::shared_ptr<Vehicle> target) :
	target_(target)
{}

Camera::~Camera()
{
	entity_->GetEvents().UnregisterEventHandler(&handler_);
}
