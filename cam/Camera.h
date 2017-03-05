#pragma once

#include <functional>

#include "Component.h"
#include "Events.h"

class Camera : public Component
{
public:
	~Camera();

protected:
	virtual void RegisterHandlers() override;

private:
	std::function<void(Events::Render)> handler_;
};

