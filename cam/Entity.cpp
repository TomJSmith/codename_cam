#include "Entity.h"

#include <algorithm>
#include <iostream>

unsigned int Entity::nextId = 0;
std::vector<std::shared_ptr<Entity>> Entity::destroyed_;

Entity::Entity() : id_(nextId++), parent_(nullptr) {}

std::shared_ptr<Entity> Entity::Create()
{
	return std::make_shared<Entity>();
}

std::weak_ptr<Entity> Entity::Create(Entity *parent)
{
	if (!parent) throw std::runtime_error("to create an entity with no parent, use Entity::Create()!");
	return parent->AddChild(std::make_shared<Entity>());
}

void Entity::Update(seconds dt) {
	auto components = components_;
	auto children = children_;

	for (auto &component : components) component->Update(dt);
	for (auto &child : children) child->Update(dt);
}

std::weak_ptr<Component> Entity::AddComponent(std::shared_ptr<Component> &&c)
{
	c->Attach(this);
	components_.push_back(c);
	return c;
}

std::weak_ptr<Entity> Entity::AddChild(std::shared_ptr<Entity> &&c)
{
	c->parent_ = this;
	children_.push_back(c);
	return c;
}

void Entity::SetParent(Entity *parent) {
	if (!parent_) throw std::runtime_error("unable to change the root entity after creation!");

	auto it = std::find_if(
		std::begin(parent_->children_),
		std::end(parent_->children_),
		[this](const auto &p) {return p.get() == this;}
	);

	if (it == std::end(parent_->children_)) throw std::runtime_error("unable to find entity in parent's children!");

	auto ptr = *it;
	parent_->children_.erase(it);
	parent_ = parent;

	if (parent_) {
		parent_->children_.push_back(ptr);
	}
}

void Entity::Destroy() {
	BroadcastEvent(Events::Destroyed {});

	auto children = children_;
	auto components = components_;

	for (auto &child : children) child->Destroy();
	for (auto &component : components) component->Destroy();

	if (parent_) {
		auto it = std::find_if(
			std::begin(parent_->children_),
			std::end(parent_->children_),
			[this](const auto &p) {return p.get() == this; }
		);

		if (it == std::end(parent_->children_)) throw std::runtime_error("unable to find entity in parent's children!");

		destroyed_.push_back(*it);
		parent_->children_.erase(it);
		parent_ = nullptr;
	}
}

void Entity::DeleteDestroyed() {
	// destroys what is (presumably) the last remaining reference to any destroyed entity
	// if a reference is held afterwards, the entity will remain alive until that reference is
	// destroyed. watchers worried about the entity being destroyed can listen for
	// Entity::DestroyEvent.
	destroyed_.clear();
}

mat4 Entity::GetGlobalTransform() const
{
	mat4 ret(1.0f);
	auto e = this;

	while (e) {
		ret = e->transform_.Matrix() * ret;
		e = e->parent_;
	}

	return ret;
}

quaternion Entity::GetGlobalRotation() const
{
	quaternion ret;
	auto e = this;

	while (e) {
		ret = e->transform_.rotation * ret;
		e = e->parent_;
	}

	return ret;
}

vec3 Entity::GetGlobalPosition() const
{
	vec3 ret = transform_.position;
	auto e = parent_;

	while (e) {
		ret = e->transform_.position + e->transform_.rotation * ret;
		e = e->parent_;
	}

	return ret;
}

void Entity::SetGlobalRotation(const quaternion &rot)
{
	transform_.rotation = (parent_ ? glm::inverse(parent_->GetGlobalRotation()) : quaternion()) * rot;
}

void Entity::SetGlobalPosition(const vec3 &pos)
{
	transform_.position = (parent_ ? -parent_->GetGlobalPosition() : vec3(0.0f, 0.0f, 0.0f)) + pos;
}