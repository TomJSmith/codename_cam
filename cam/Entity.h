#pragma once

#include <memory>
#include <vector>

#include "Component.h"
#include "EventSystem.h"
#include "Time.h"
#include "Transform.h"

class Entity {
 public:
	Entity(Entity *parent = nullptr);

	void Update(seconds dt);
	void AddComponent(std::shared_ptr<Component> c);
	void SetParent(Entity *parent);

	Entity *GetParent() { return parent_; }
	EventSystem &GetEvents() { return events_; }
	Transform &GetTransform() { return transform_; }

	mat4 GetGlobalTransform() const;

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

	std::vector<std::shared_ptr<Component>> components_;
	std::vector<std::shared_ptr<Entity>> children_;

	Entity *parent_;

	EventSystem events_;
	Transform transform_;

	static unsigned int nextId;
};
