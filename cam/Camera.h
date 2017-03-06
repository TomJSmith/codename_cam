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
	Camera(std::shared_ptr<Vehicle> target);
	~Camera();

protected:
	virtual void RegisterHandlers() override;

private:
	std::function<void(Events::Render)> handler_;
	//Physics &physics_;
	std::weak_ptr<Vehicle> target_;
	//std::weak_ptr<Entity> child_;
};

