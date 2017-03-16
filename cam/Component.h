#pragma once

#include "Time.h"

class Entity;

class Component {
 public:
	Component();
	virtual ~Component();

	void Attach(Entity *entity);
	virtual void Update(seconds dt) {};
	virtual void Destroy() {};

	int Id() { return id_; }
	Entity *entity() { return entity_; }

 protected:
	int id_;
	Entity *entity_;

	virtual void RegisterHandlers() {}

 private:
	static int nextId;
};
