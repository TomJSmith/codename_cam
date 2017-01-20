#ifndef __COMPONENT_H_
#define __COMPONENT_H_

#include "Time.h"

class Entity;

class Component {
 public:
	Component();

	void Attach(Entity *entity);
	virtual void Update(seconds dt) {};

	int Id() { return id_; }
	Entity *entity() { return entity_; }
 protected:
	int id_;
	Entity *entity_;

	virtual void RegisterHandlers() {}

 private:
	static int nextId;
};

#endif // __COMPONENT_H_
