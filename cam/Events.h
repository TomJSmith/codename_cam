#pragma once

#include <boost/python.hpp>

#include "EventSystem.h"
#include "Renderer.h"

class Entity;

using namespace boost;

namespace Events {
	struct Infected {};
	struct Destroyed {};
	struct RunnerCreated {
		Entity *runner;
		Entity *GetRunner() { return runner; }
	};

	struct Render {
		std::vector<Renderer::MeshData> &data;
		mat4 &cameraTransform;
	};

	struct Collided {
		Entity *other;
		Entity *GetOther() { return other; } // for python
	};

	struct StartGame {};

	struct ScriptEvent {
		python::object pyevent;
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
	Events::Infected,
	Events::RunnerCreated,
	Events::StartGame,
	Events::ScriptEvent
>;