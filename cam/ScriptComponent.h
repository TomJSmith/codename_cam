#pragma once

#include <boost/python.hpp>

#include <map>

#include "Component.h"

class ScriptComponent : public Component {
 public:
	ScriptComponent(const std::string &filename);
	void Update(seconds dt) override;

 private:
	boost::python::object locals_;
	boost::python::object globals_;
	boost::python::object main_;

	std::string type_;

	void InitScript(const std::string &filename);
	static void InitPython();
};

