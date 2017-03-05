#pragma once

#include <boost/python.hpp>

#include <map>

#include "Component.h"

class Physics;

class ScriptComponent : public Component {
 public:
	ScriptComponent(const std::string &filename, Physics &physics);
	~ScriptComponent();

	virtual void Update(seconds dt) override;

	Physics &GetPhysics() const { return physics_; }

 private:
	boost::python::dict locals_;
	boost::python::object main_;

	std::string type_;

	Physics &physics_;

	virtual void RegisterHandlers() override;
	void InitScript(const std::string &filename);
	static void InitPython();

	std::function<void(Events::Collided)> handler_;
};

