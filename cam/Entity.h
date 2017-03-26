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

	// TODO create with position
	static std::shared_ptr<Entity> Create();
	static std::weak_ptr<Entity> Create(Entity *parent);
	static void DeleteDestroyed();

	void Update(seconds dt);

	// Add a child or a component to this entity.
	//
	// Note that these return a weak_ptr, and take the shared_ptr arguments as rvalue
	// references. What this means is that those shared_ptrs are no longer valid after
	// they've been added to an entity - you must refer to the entity/component using
	// the returned weak_ptr.
	//
	// This ensures that entities are properly deleted when they are removed from
	// the scene graph. Since nobody else has a shared_ptr to any of an entity's
	// children or components, they are destroyed when the entity is, but others can
	// still reference them via these weak_ptrs.
	//
	// This means two things for usage of this class:
	//   - You can't pass a shared_ptr directly to these functions - you have to explicitly
	//     move them in, or pass in a temporary.
	//     eg: 
	//         entity.AddChild(std::make_shared<Entity>())
	//     or:
	//         std::shared_ptr<Entity> child; entity.AddChild(std::move(child));
	//
	//   - You can't use these weak_ptrs without checking if they are still valid.
	//     eg:
	//         auto child = entity.AddComponent(...);
	//         if (auto c = child.lock()) { /* use c */ }
	std::weak_ptr<Entity> AddChild(std::shared_ptr<Entity> &&c);
	std::weak_ptr<Component> AddComponent(std::shared_ptr<Component> &&c);

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
	void RegisterEventHandler(T handler)
	{
		events_.RegisterEventHandler(handler);
	}

	template <class T>
	void UnregisterEventHandler(T handler)
	{
		events_.UnregisterEventHandler(handler);
	}

	template <class T>
	void BroadcastEvent(T event)
	{
		FireEvent(event);
		for (auto &child : children_) child->BroadcastEvent(event);
	}

	template <class T>
	void FireEvent(T event)
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
