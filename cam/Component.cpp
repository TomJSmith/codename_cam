#include "Component.h"

int Component::nextId = 0;

Component::Component() : id_(nextId++), entity_(nullptr) {}
Component::~Component() {}

void Component::Attach(Entity *entity)
{
	entity_ = entity;
	RegisterHandlers();
}