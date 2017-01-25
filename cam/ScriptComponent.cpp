#include <boost/python.hpp>

#include <string>

#include "ScriptComponent.h"

using namespace boost;

BOOST_PYTHON_MODULE(scriptcomponent) {
	python::class_<ScriptComponent>("ScriptComponent", python::init<std::string>())
		.add_property("id", &Component::Id);
}

ScriptComponent::ScriptComponent(const std::string &type) :
	type_(type)
{
	InitPython();
	InitScript(type_);
}

void ScriptComponent::Update(seconds dt) {
	std::string script = "update(obj, " + std::to_string(dt.count()) + ")";
	python::exec(script.c_str(), globals_, locals_);
}

void ScriptComponent::InitPython() {
	static bool initialized = false;

	if (!initialized) {
		try {
			Py_SetPythonHome(".");
			Py_Initialize();
			initscriptcomponent();

			initialized = true;
		} catch (const python::error_already_set &) {
			PyErr_Print();
		}
	}
}

void ScriptComponent::InitScript(const std::string &type) {
	try {
		std::string filename = type + ".py";

		auto module = python::borrowed(PyImport_AddModule("__main__"));
		auto handle = python::handle<>(module);
		main_ = python::object(handle);

		globals_ = python::dict(main_.attr("__dict__"));
		locals_ = python::dict(main_.attr("__dict__"));
		locals_["obj"] = python::ptr(this);

		python::exec_file(filename.c_str(), globals_, locals_);
	} catch (const python::error_already_set &) {
		PyErr_Print();
	}
}

// Workaround for known MSVC bug
namespace boost {
	template <>
	ScriptComponent const volatile * get_pointer<class ScriptComponent const volatile>(
		class ScriptComponent const volatile *c
	) {
		return c;
	}
}
