#include <boost/python.hpp>

#include <iostream>
#include <string>

#include "Entity.h"
#include "Physics.h"
#include "ScriptComponent.h"
#include "Vehicle.h"
#include "Controller.h"

using namespace boost;

float dot(const vec3 &a, const vec3 &b) {
	return glm::dot(a, b);
}

float length(const vec3 &v) {
	return glm::length(v);
}

BOOST_PYTHON_MODULE(physics) {
	python::class_<PxVec3>("PxVec3")
		.def(python::init<float, float, float>())
		.def_readwrite("x", &PxVec3::x)
		.def_readwrite("y", &PxVec3::y)
		.def_readwrite("z", &PxVec3::z);

	python::class_<vec3>("Vec3")
		.def(python::init<float, float, float>())
		.def_readwrite("x", &vec3::x)
		.def_readwrite("y", &vec3::y)
		.def_readwrite("z", &vec3::z)
		.def("length", &length)
		.def(python::self + python::self)
		.def(python::self - python::self)
		.def(python::self += python::self)
		.def(python::self -= python::self)
		.def("dot", &dot)
		.staticmethod("dot");

	python::class_<quaternion>("Quaternion")
		.def("axis_angle", &glm::angleAxis<float, glm::defaultp>)
		.staticmethod("axis_angle");

	python::class_<Transform>("Transform")
		.def_readwrite("position", &Transform::position)
		.def_readwrite("rotation", &Transform::rotation)
		.def("forward", &Transform::Forward)
		.def("up", &Transform::Up)
		.def("right", &Transform::Right)
		.def("global_position", &Transform::GlobalPosition);

	python::class_<Physics>("Physics");
}

BOOST_PYTHON_MODULE(controller) {
	python::class_<Controller, std::shared_ptr<Controller>>("Controller", python::init<int>());
}

BOOST_PYTHON_MODULE(aicontroller) {
	python::class_<aiController, std::shared_ptr<aiController>>("aiController", python::init<int>());
}

// boost::python won't let us use shared_ptr<Component> for subclasses of Component by
// default. This means when a script creates a Vehicle, and calls AddComponent, it
// passes a shared_ptr<Vehicle> instead of a shared_ptr<Component>.
// So we convert to a shared_ptr<Component> and give it to the entity to manage.
// This makes sure Python doesn't try to delete the object on us after the script is
// done with it.
template <class T>
void AddComponent(Entity &e, std::shared_ptr<T> c) {
	e.AddComponent(std::shared_ptr<Component>(c));
}

BOOST_PYTHON_MODULE(entity) {
	python::class_<Entity>("Entity")
		// For every type of component we want to create in scripts, we need to
		// add an overload here
		.def("add_component", AddComponent<Vehicle>)
		.def("transform", &Entity::GetTransform, python::return_internal_reference<>());
}

BOOST_PYTHON_MODULE(component) {
	python::class_<Component>("Component");

	python::class_<ScriptComponent, python::bases<Component>>("ScriptComponent", python::init<std::string, Physics &>())
		.add_property("id", &Component::Id)
		.def("entity", &Component::entity, python::return_internal_reference<>())
		.def("physics", &ScriptComponent::GetPhysics, python::return_internal_reference<>());
}

BOOST_PYTHON_MODULE(vehicle) {
	python::class_<PxVehicleDrive4WRawInputData>("Input")
		.def("set_acceleration", &PxVehicleDrive4WRawInputData::setDigitalAccel)
		.def("set_steer_left", &PxVehicleDrive4WRawInputData::setDigitalSteerLeft)
		.def("set_steer_right", &PxVehicleDrive4WRawInputData::setDigitalSteerRight)
		.def("set_brake", &PxVehicleDrive4WRawInputData::setDigitalBrake);

	python::class_<Vehicle, std::shared_ptr<Vehicle>, python::bases<Component>>
		("Vehicle", python::init<Physics &, std::shared_ptr<Controller>, Vehicle::Configuration &>())
		.def(python::init<Physics &, std::shared_ptr<aiController>, Vehicle::Configuration &>());

	python::class_<Vehicle::Configuration>("Configuration")
		.def_readwrite("position", &Vehicle::Configuration::position)
		.def_readwrite("wheel_mass", &Vehicle::Configuration::wheelMass)
		.def_readwrite("wheel_moi", &Vehicle::Configuration::wheelMOI)
		.def_readwrite("wheel_radius", &Vehicle::Configuration::wheelRadius)
		.def_readwrite("wheel_width", &Vehicle::Configuration::wheelWidth)
		.def_readwrite("n_wheels", &Vehicle::Configuration::nWheels)
		.def_readwrite("chassis_offset", &Vehicle::Configuration::chassisOffset)
		.def_readwrite("chassis_dimensions", &Vehicle::Configuration::chassisDimensions)
		.def_readwrite("chassis_moi", &Vehicle::Configuration::chassisMOI)
		.def_readwrite("chassis_mass", &Vehicle::Configuration::chassisMass)
		.def_readwrite("handbrake_torque", &Vehicle::Configuration::handbrakeTorque)
		.def_readwrite("steer_angle", &Vehicle::Configuration::steerAngle)
		.def_readwrite("max_compression", &Vehicle::Configuration::maxCompression)
		.def_readwrite("max_droop", &Vehicle::Configuration::maxDroop)
		.def_readwrite("suspension_strength", &Vehicle::Configuration::suspensionStrength)
		.def_readwrite("suspension_damping", &Vehicle::Configuration::suspensionDamping)
		.def_readwrite("camber_at_rest", &Vehicle::Configuration::camberAtRest)
		.def_readwrite("camber_at_max_droop", &Vehicle::Configuration::camberAtMaxDroop)
		.def_readwrite("camber_at_max_compression", &Vehicle::Configuration::camberAtMaxCompression)
		.def_readwrite("torque", &Vehicle::Configuration::torque)
		.def_readwrite("max_omega", &Vehicle::Configuration::maxOmega);
}

ScriptComponent::ScriptComponent(const std::string &type, Physics &physics) :
	type_(type),
	physics_(physics)
{
	InitPython();
}

void ScriptComponent::RegisterHandlers()
{
	InitScript(type_);
}

void ScriptComponent::Update(seconds dt)
{
	try {
		if (locals_.has_key("update")) {
			locals_["update"](python::ptr(this), dt.count());
		}
	} catch (const python::error_already_set &) {
		PyErr_Print();
	}
}

void ScriptComponent::InitPython()
{
	static bool initialized = false;

	if (!initialized) {
		try {
			Py_SetPythonHome(".");
			Py_Initialize();

			initcomponent();
			initentity();
			initphysics();
			initvehicle();
			initcontroller();
			initaicontroller();

			initialized = true;
		} catch (const python::error_already_set &) {
			PyErr_Print();
		}
	}
}

void ScriptComponent::InitScript(const std::string &type)
{
	try {
		std::string filename = type + ".py";

		auto module = python::borrowed(PyImport_AddModule("__main__"));
		auto handle = python::handle<>(module);
		main_ = python::object(handle);
		locals_ = python::dict(main_.attr("__dict__"));

		python::exec_file(filename.c_str(), locals_, locals_);

		if (locals_.has_key("init")) {
			locals_["init"](python::ptr(this));
		}
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
	
	template <>
	Component const volatile * get_pointer<class Component const volatile>(
		class Component const volatile *c
		) {
		return c;
	}

	template <>
	Vehicle const volatile * get_pointer<class Vehicle const volatile>(
		class Vehicle const volatile *c
		) {
		return c;
	}

	template <>
	PxVehicleDrive4WRawInputData const volatile * get_pointer<class PxVehicleDrive4WRawInputData const volatile>(
		class PxVehicleDrive4WRawInputData const volatile *c
		) {
		return c;
	}
}
