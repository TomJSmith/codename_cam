#pragma once
#include "System.h"
#include "Component.h"
#include "Events.h"
#include "Transform.h"
#include "Entity.h"

class Runner :
	public Component
{
public:
	Runner();
	~Runner();
protected:
	virtual void RegisterHandlers() override;
};

