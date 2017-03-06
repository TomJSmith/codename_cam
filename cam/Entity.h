#pragma once

#include <memory>
#include <vector>

#include "Events.h"
#include "Component.h"
#include "Time.h"
#include "Transform.h"
#include "windows.h"
#include "Xinput.h"

class Entity {
 public:
	Entity();
	static std::shared_ptr<Entity> Create(Entity *parent);
	static void DeleteDestroyed();

	void Update(seconds dt);
	void AddChild(std::shared_ptr<Entity> c);
	void AddComponent(std::shared_ptr<Component> c);
	void SetParent(Entity *parent);

	// Mark an entity for destruction at the next call to DeleteDestroyed().
	// Note that while this does remove the entity from the scene graph, along
	// with all its children, and send a DestroyEvent to any listeners, it does
	// *not* delete the entity, or its components. This prevents bugs when destroying
	// objects in Update() - those objects might still have Update() called later this
	// frame, so we can't delete them until the end of the frame.
	void Destroy();

	unsigned int Id() { return id_; }
	Entity *GetParent() { return parent_; }
	EventSystem &GetEvents() { return events_; }
	Transform &GetTransform() { return transform_; }

	mat4 GetGlobalTransform() const;
	quaternion GetGlobalRotation() const;
	vec3 GetGlobalPosition() const;

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
	static std::vector<std::shared_ptr<Entity>> destroyed_;
};
