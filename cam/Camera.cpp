#include "Camera.h"

#include <glm/gtx/transform.hpp>
#include <iostream>

#include "Entity.h"
#include "Physics.h"
#include "RigidBody.h"

void Camera::RegisterHandlers()
{
	handler_ = [this] (Events::Render e) {
		auto pos = glm::translate(entity_->GetGlobalPosition());
		auto rot = glm::toMat4(entity_->GetGlobalRotation() * glm::angleAxis(PxPi, vec3(0.0f, 1.0f, 0.0f)));
		e.cameraTransform = pos * rot;
	};

	entity_->GetEvents().RegisterEventHandler(&handler_);
}

Camera::Camera(Physics &physics) :
	physics_(physics)
{}

Camera::~Camera()
{
	entity_->GetEvents().UnregisterEventHandler(&handler_);
}