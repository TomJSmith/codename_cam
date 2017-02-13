#include "Entity.h"

#include <algorithm>

unsigned int Entity::nextId = 0;

Entity::Entity() : id_(nextId++), parent_(nullptr) {}

std::shared_ptr<Entity> Entity::Create(Entity *parent)
{
	auto ret = std::make_shared<Entity>();

	if (parent) {
		parent->AddChild(ret);
	}

	return ret;
}

void Entity::Update(seconds dt) {
	auto components = components_;
	auto children = children_;

	for (auto &component : components) component->Update(dt);
	for (auto &child : children) child->Update(dt);
}

void Entity::AddComponent(std::shared_ptr<Component> c)
{
	c->Attach(this);
	components_.push_back(std::move(c));
}

void Entity::AddChild(std::shared_ptr<Entity> c)
{
	c->parent_ = this;
	children_.push_back(std::move(c));
}

void Entity::SetParent(Entity *parent) {
	if (!parent_) throw std::runtime_error("unable to change the root entity after creation!");

	auto it = std::find_if(
		std::begin(parent_->children_),
		std::end(parent_->children_),
		[=](const auto &p) {return p.get() == this;}
	);

	if (it == std::end(parent_->children_)) throw std::runtime_error("unable to find entity in parent's children!");

	auto ptr = *it;
	parent_->children_.erase(it);
	parent_ = parent;

	if (parent_) {
		parent_->children_.push_back(ptr);
	}
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