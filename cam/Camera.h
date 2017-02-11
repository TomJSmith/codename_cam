#pragma once

#include "Component.h"

class Camera : public Component
{
protected:
	virtual void RegisterHandlers() override;
};

