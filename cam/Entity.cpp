#include "Entity.h"

#include <algorithm>

unsigned int Entity::nextId = 0;

Entity::Entity(Entity *parent) : id_(nextId++)
{
	SetParent(parent);
}

void Entity::Update(seconds dt) {
	for (auto &component : components_) component->Update(dt);
	for (auto &child : children_) child->Update(dt);
}

void Entity::AddComponent(std::unique_ptr<Component> c) {
	c->Attach(this);
	components_.push_back(std::move(c));
}

void Entity::SetParent(Entity *parent) {
	if (parent_) {
		auto it = std::find_if(
			std::begin(parent_->children_),
			std::end(parent_->children_),
			[=](const auto &p) {return p.get() == this;}
		);

		if (it == std::end(parent_->children_)) throw std::runtime_error("unable to find entity in parent's children!");

		if (parent) {
			parent->children_.push_back(*it);
		}

		parent_->children_.erase(it);
	}
	else {
		if (parent) {
			parent->children_.push_back(std::shared_ptr<Entity>(this));
		}
	}
}