#pragma once

#include <functional>
#include <memory>

#include "Component.h"
#include "Events.h"
#include "Vehicle.h"

class Physics;
class RigidBody;

class Camera : public Component
{
public:
	Camera(Physics &physics);
	~Camera();

protected:
	virtual void RegisterHandlers() override;

private:
	std::function<void(Events::Render)> handler_;
	Physics &physics_;

	std::weak_ptr<Entity> target_;
	std::weak_ptr<Entity> cam_;

	PxRigidDynamic *camrb_;
	PxRigidDynamic *targetrb_;
};

