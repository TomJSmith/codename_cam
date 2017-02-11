#include "Camera.h"

#include "Entity.h"

void Camera::RegisterHandlers()
{
	entity_->GetEvents().RegisterEventHandler([this] (Renderer::RenderEvent e) {
		e.cameraTransform = entity_->GetGlobalTransform();
	});
}
