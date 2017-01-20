#ifndef __EVENT_SYSTEM_H_
#define __EVENT_SYSTEM_H_

#include <functional>
#include <vector>

#include "Mesh.h"

template <class... EventTypes>
class EventSystem_ {
public:
	EventSystem_() {}
	void FireEvent() {}
	void RegisterEventHandler() {}
};

template <class EventType, class... EventTypes>
class EventSystem_<EventType, EventTypes...> : public EventSystem_<EventTypes...> {
public:
	// Make base class handler functions visible in this class.
	// Necessary for overloads to resolve correctly - otherwise, the most
	// specific subclass's functions are always called, regardless of type.
	using EventSystem_<EventTypes...>::FireEvent;
	using EventSystem_<EventTypes...>::RegisterEventHandler;

	// Call all registered handlers for @EventType with @event as the parameter.
	void FireEvent(EventType event) {
		for (const auto &handler : handlers_) handler(event);
	}

	// Add the function @handler to the list of handlers for @EventType.
	// This takes an std::function which can be created from a lambda, function
	// pointer or functor (callable struct) - so any of these work:
	//
	// RegisterEventHandler([] (EventType e) { /* handle event */ });
	//
	// void Handler(EventType e) { /* handle event */ };
	// RegisterEventHandler(handler);
	//
	// struct Handler {
	//   void operator()(EventType e) { /* handle event */ };
	// };
	// RegisterEventHandler(Handler());
	void RegisterEventHandler(std::function<void(EventType)> handler) {
		handlers_.push_back(handler);
	}

private:
	std::vector<std::function<void(EventType)>> handlers_;
};

// Expose the template class with all event types as EventSystem (so we don't have to
// write EventSystem<Event1, Event2, ...> everywhere).
//
// To add an event type, add it to the template arguments here.
using EventSystem = EventSystem_<Mesh::GetMeshDataEvent>;

#endif // __EVENT_SYSTEM_H_
