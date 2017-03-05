#pragma once

#include "EventSystem.h"
#include "Renderer.h"

class Entity;

namespace Events {
	struct Infected {};
	struct Destroyed {};

	struct Render {
		std::vector<Renderer::MeshData> &data;
		mat4 &cameraTransform;
	};

	struct Collided {
		Entity *other;
		Entity *GetOther() { return other; } // for python
	};
};

// Expose the template class with all event types as EventSystem (so we don't have to
// write EventSystem<Event1, Event2, ...> everywhere).
//
// To add an event type, add it to the template arguments here.
using EventSystem = EventSystem_<
	Events::Render,
	Events::Collided, 
	Events::Destroyed, 
	Events::Infected
>;