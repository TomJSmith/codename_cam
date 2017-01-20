#ifndef __ENTITY_H_
#define __ENTITY_H_

#include "Time.h"
#include <memory>
#include <vector>

#include "Component.h"
#include "EventSystem.h"
#include "Transform.h"

class Entity {
 public:
	Entity(Entity *parent = nullptr);
	void Update(seconds dt);
	void AddComponent(std::unique_ptr<Component> c);
	void SetParent(Entity *parent);

	Entity *Parent() { return parent_; }
	// TODO why are these lower case...
	EventSystem &events() { return events_; }
	Transform &transform() { return transform_; }

	template <typename T>
	void RegisterEventHandler(std::function<void(T)> handler)
	{
		events_.RegisterEventHandler(handler);
	}

	template <typename T>
	void BroadcastEvent(T &event)
	{
		FireEvent(event);
		for (auto &child : children_) child->BroadcastEvent(event);
	}

	template <typename T>
	void FireEvent(T &event)
	{
		events_.FireEvent(event);
	}
 private:
	int id_;

	std::vector<std::unique_ptr<Component>> components_;
	std::vector<std::shared_ptr<Entity>> children_;

	Entity *parent_;

	EventSystem events_;
	Transform transform_;

	static unsigned int nextId;
};

#endif // __ENTITY_H_
