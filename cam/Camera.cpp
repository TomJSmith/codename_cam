#include "Camera.h"

#include <glm/gtx/transform.hpp>
#include <iostream>

#include "Entity.h"
#include "Physics.h"
#include "RigidBody.h"

void Camera::RegisterHandlers()
{
	auto c = Entity::Create(entity_);
	std::weak_ptr<Entity> child = c;

	handler_ = [this, child](Events::Render e) {
		if (auto c = child.lock()) {
			auto pos = glm::translate(c->GetGlobalPosition());
			auto rot = glm::toMat4(entity_->GetGlobalRotation() * glm::angleAxis(PxPi, vec3(0.0f, 1.0f, 0.0f)));

			e.cameraTransform = pos * rot;
		}
	};

	entity_->GetEvents().RegisterEventHandler(&handler_);

	auto p = entity_->GetTransform().position;
	PxVec3 offset(0, 5, -20);

	auto gp = entity_->GetGlobalPosition();
	auto gr = entity_->GetGlobalRotation();

	if (auto t = target_.lock()) {
		auto &phys = t->GetPhysics();

		std::shared_ptr<RigidBody> body(new RigidBody(
			phys,
			*phys.GetPhysics()->createMaterial(0.0f, 0.0f, 1.0f),
			PxSphereGeometry(3.0f),
			PxTransform(PxVec3(gp.x + offset.x, gp.y + offset.y, gp.z + offset.z))
		));

		c->AddComponent(body);

		auto rb = body->GetRigidBody();
		rb->is<PxRigidDynamic>()->setMass(0.001f);
		auto tactor = t->GetActor();

		auto d6 = PxD6JointCreate(
			*phys.GetPhysics(),
			tactor,
			PxTransform(PxVec3(0.0f, 0.0f, 0.0f)),
			rb,
			PxTransform(-offset)
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
