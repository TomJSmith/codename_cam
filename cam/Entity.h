#pragma once

#include <memory>
#include <vector>

#include "Component.h"
#include "EventSystem.h"
#include "Time.h"
#include "Transform.h"
#include "windows.h"
#include "Xinput.h"

class Entity {
 public:
	Entity();
	static std::shared_ptr<Entity> Create(Entity *parent);

	void Update(seconds dt);
	void AddChild(std::shared_ptr<Entity> c);
	void AddComponent(std::shared_ptr<Component> c);
	void SetParent(Entity *parent);

	unsigned int Id() { return id_; }
	Entity *GetParent() { return parent_; }
	EventSystem &GetEvents() { return events_; }
	Transform &GetTransform() { return transform_; }

	mat4 GetGlobalTransform() const;

	template <class T>
	void RegisterEventHandler(std::function<void(T)> handler)
	{
		events_.RegisterEventHandler(handler);
	}

	template <class T>
	void BroadcastEvent(T &event)
	{
		FireEvent(event);
		for (auto &child : children_) child->BroadcastEvent(event);
	}

	template <class T>
	void FireEvent(T &event)
	{
		events_.FireEvent(event);
	}
 private:
	
	unsigned int id_;

	std::vector<std::shared_ptr<Component>> components_;
	std::vector<std::shared_ptr<Entity>> children_;

	Entity *parent_;

	EventSystem events_;
	Transform transform_;

	static unsigned int nextId;
};
