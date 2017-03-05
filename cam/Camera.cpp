#include "Camera.h"

#include "Entity.h"

void Camera::RegisterHandlers()
{
	handler_ = [this](Events::Render e) {
		e.cameraTransform = entity_->GetGlobalTransform();
	};

	entity_->GetEvents().RegisterEventHandler(&handler_);
}

Camera::~Camera()
{
	entity_->GetEvents().UnregisterEventHandler(&handler_);
}
