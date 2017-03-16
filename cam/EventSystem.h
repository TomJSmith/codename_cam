#pragma once

#include <algorithm>
#include <functional>
#include <vector>

template <class... EventTypes>
class EventSystem_ {
public:
	EventSystem_() {}
	void FireEvent() {}
	void RegisterEventHandler() {}
	void UnregisterEventHandler() {}
};

template <class EventType, class... EventTypes>
class EventSystem_<EventType, EventTypes...> : public EventSystem_<EventTypes...> {
public:
	// Make base class handler functions visible in this class.
	// Necessary for overloads to resolve correctly - otherwise, the most
	// specific subclass's functions are always called, regardless of type.
	using EventSystem_<EventTypes...>::FireEvent;
	using EventSystem_<EventTypes...>::RegisterEventHandler;
	using EventSystem_<EventTypes...>::UnregisterEventHandler;

	// Call all registered handlers for @EventType with @event as the parameter.
	void FireEvent(EventType event) {
		for (const auto &handler : handlers_) (*handler)(event);
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
	void RegisterEventHandler(std::function<void(EventType)> *handler) {
		handlers_.push_back(handler);
	}

	void UnregisterEventHandler(std::function<void(EventType)> *handler) {
		handlers_.erase(std::remove(handlers_.begin(), handlers_.end(), handler), handlers_.end());
	}
private:
	std::vector<std::function<void(EventType)> *> handlers_;
};
