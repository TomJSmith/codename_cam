#ifndef __SCRIPT_COMPONENT_H_
#define __SCRIPT_COMPONENT_H_

#define BOOST_PYTHON_STATIC_LIB

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

#endif // __SCRIPT_COMPONENT_H_
