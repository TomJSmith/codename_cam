#include "ScriptComponent.h"

#include <boost/python.hpp>

#include <iostream>
#include <locale>
#include <string>

#include "Camera.h"
#include "Controller.h"
#include "Entity.h"
#include "Image.h"
#include "ImageShader.h"
#include "Mesh.h"
#include "ModelShader.h"
#include "NavMesh.h"
#include "Physics.h"
#include "RigidBody.h"
#include "Runner.h"
#include "Text.h"
#include "Trigger.h"
#include "Util.h"
#include "Vehicle.h"

using namespace boost;

float dot3(const vec3 &a, const vec3 &b) {
	return glm::dot(a, b);
}

float dot2(const vec2 &a, const vec2 &b) {
	return glm::dot(a, b);
}

float length(const vec3 &v) {
	return glm::length(v);
}

vec3 normal(const vec3 &v) {
	return glm::normalize(v);
}

std::shared_ptr<PxRaycastHit> Raycast(Physics &physics, vec3 origin, vec3 direction, float dist)
{
	std::shared_ptr<PxRaycastHit> ret;
	PxRaycastBuffer hit;
	if (physics.GetScene()->raycast(PxVec3(origin.x, origin.y, origin.z), PxVec3(direction.x, direction.y, direction.z), dist, hit, PxHitFlags(PxHitFlag::eDEFAULT), PxQueryFilterData(PxFilterData{ 0, 0, 0, Physics::DRIVABLE_SURFACE }, PxQueryFlag::eDYNAMIC | PxQueryFlag::eSTATIC))) {
		if (hit.getNbTouches() > 0) {
			ret.reset(new PxRaycastHit(hit.getTouch(0)));
		}
		else {
			ret.reset(new PxRaycastHit(hit.block));
		}
	}

	return ret;
}

BOOST_PYTHON_MODULE(physics) {
	python::class_<PxVec3>("PxVec3")
		.def(python::init<float, float, float>())
		.def_readwrite("x", &PxVec3::x)
		.def_readwrite("y", &PxVec3::y)
		.def_readwrite("z", &PxVec3::z);

	python::class_<PxTransform>("PxTransform", python::init<PxVec3>())
		.def(python::init<PxVec3, PxQuat>());

	python::class_<PxGeometry>("PxGeometry", python::no_init);
	python::class_<PxBoxGeometry, python::bases<PxGeometry>>("PxBox", python::init<PxVec3>());
	python::class_<PxSphereGeometry, python::bases<PxGeometry>>("PxSphere", python::init<float>());

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
		.def(-python::self)
		.def(python::self * float())
		.def(python::self / float())
		.def(float() * python::self)
		.def(float() / python::self)
		.def(python::self *= float())
		.def(python::self /= float())
		.def("dot", &dot3)
		.def("normal", &normal)
		.staticmethod("dot");

	python::class_<vec2>("Vec2")
		.def(python::init<float, float>())
		.def_readwrite("x", &vec2::x)
		.def_readwrite("y", &vec2::y)
		.def("length", &length)
		.def(python::self + python::self)
		.def(python::self - python::self)
		.def(python::self += python::self)
		.def(python::self -= python::self)
		.def(-python::self)
		.def(python::self * float())
		.def(python::self / float())
		.def(float() * python::self)
		.def(float() / python::self)
		.def(python::self *= float())
		.def(python::self /= float())
		.def("dot", &dot2)
		.staticmethod("dot");

	python::class_<quaternion>("Quaternion")
		.def(python::self * vec3())
		.def("axis_angle", &glm::angleAxis<float, glm::defaultp>)
		.staticmethod("axis_angle")
		.def("look_at", &LookAt)
		.staticmethod("look_at");

	python::class_<PxQuat>("PxQuat")
		.def(python::init<float, float, float, float>())
		.def_readwrite("x", &PxQuat::x)
		.def_readwrite("y", &PxQuat::y)
		.def_readwrite("z", &PxQuat::z)
		.def_readwrite("w", &PxQuat::w);

	python::class_<Transform>("Transform")
		.def_readwrite("position", &Transform::position)
		.def_readwrite("rotation", &Transform::rotation)
		.def("forward", &Transform::Forward)
		.def("up", &Transform::Up)
		.def("right", &Transform::Right)
		.def("global_position", &Transform::GlobalPosition);

	python::class_<Physics>("Physics")
		.def("raycast", &Raycast);

	python::class_<PxRaycastHit, std::shared_ptr<PxRaycastHit>>("RaycastHit")
		.add_property("position", &PxRaycastHit::position);
}

BOOST_PYTHON_MODULE(controller) {
	python::class_<Controller, std::shared_ptr<Controller>>("Controller", python::init<int>())
		.add_property("acceleration", &Controller::getAcceleration)
		.add_property("brake", &Controller::getBrake)
		.add_property("direction", &Controller::getDirectional)
		.add_property("select", &Controller::getSelect)
		.add_property("secondary", &Controller::getSecondary)
		.def("update", &Controller::UpdateState);
}

BOOST_PYTHON_MODULE(aicontroller) {
	python::class_<aiController, python::bases<Controller>, std::shared_ptr<aiController>>("aiController", python::init<int>())
		.def("setDirection", &aiController::setDirection)
		.def("setBrake", &aiController::setBrake)
		.def("setReverse", &aiController::setReverse)
		.def("setAccel", &aiController::setAccel);
}

//BOOST_PYTHON_MODULE(runner) {
	////python::class_<Runner, std::shared_ptr<Runner>>("Runner", python::init<>());
//}


BOOST_PYTHON_MODULE(navmesh) {
	python::class_<NavMesh, std::shared_ptr<NavMesh>>("NavMesh", python::init<const char*, vec3>())
		.def("getSimpleGraph", &NavMesh::getSimpleGraph);
}

BOOST_PYTHON_MODULE(ui) {
	python::class_<Image, python::bases<Component>, std::shared_ptr<Image>>("Image", python::init<const char *, const vec2 &, const vec2 &, size_t>())
		.add_property("size", &Image::GetSize, &Image::SetSize)
		.add_property("position", &Image::GetPosition, &Image::SetPosition)
		.def("set_layer", &Image::SetLayer);

	python::class_<Text, python::bases<Component>, std::shared_ptr<Text>>("Text", python::init<std::string, const std::string &, vec2, size_t>())
		.add_property("position", &Text::GetPosition, &Text::SetPosition)
		.add_property("text", &Text::GetText, &Text::SetText);
}

// boost::python won't let us use shared_ptr<Component> for subclasses of Component by
// default. This means when a script creates a Vehicle, and calls AddComponent, it
// passes a shared_ptr<Vehicle> instead of a shared_ptr<Component>.
// So we convert to a shared_ptr<Component> and give it to the entity to manage.
// This makes sure Python doesn't try to delete the object on us after the script is
// done with it.
template <class T>
std::weak_ptr<Component> AddComponent(Entity &e, std::shared_ptr<T> c) {
	return e.AddComponent(std::shared_ptr<Component>(c));
}

std::weak_ptr<Component> AddScriptComponent(Entity &e, boost::python::object component, Physics &physics) {
	return e.AddComponent(std::make_shared<ScriptComponent>(component, physics));
}

// For this, we just need a template so we can pass a Python function (as python::object)
// to the handler of the right type
template <class T>
void RegisterEventHandler(Entity &e, python::object pyhandler) {
	auto handler = [=](T event) {
		try {
			pyhandler(event);
		}
		catch (const python::error_already_set &) {
			PyErr_Print();
		}
	};

	// TODO leaky leaky
	e.RegisterEventHandler(new std::function<void(T)>(handler));
}

void RegisterScriptEventHandler(Entity &e, python::object type, python::object pyhandler) {
	auto name = type.attr("__name__");

	auto handler = [=](Events::ScriptEvent event) {
		if (event.pyevent.attr("__class__").attr("__name__") == name) {
			try {
				pyhandler(event.pyevent);
			}
			catch (const python::error_already_set &) {
				PyErr_Print();
			}
		}
	};

	// TODO this is some grease right hurrrrr
	e.RegisterEventHandler(new std::function<void(Events::ScriptEvent)>(handler));
}

void BroadcastScriptEvent(Entity &e, python::object event) {
	try {
		e.BroadcastEvent(Events::ScriptEvent{ event });
	}
	catch (const python::error_already_set &) {
		PyErr_Print();
	}
}

void FireScriptEvent(Entity &e, python::object event) {
	e.FireEvent(Events::ScriptEvent{ event });
}

BOOST_PYTHON_MODULE(events) {
	//python::class_<Events::RunnerCreated>("RunnerCreated")
	//	.def("get_runner", &Events::RunnerCreated::GetRunner, python::return_internal_reference<>());
	python::class_<Events::Infected>("Infected")
		.def_readwrite("other", &Events::Infected::other)
		.def("getother", &Events::Infected::GetOther, python::return_internal_reference<>());
	python::class_<Events::Destroyed>("Destroyed");
//	python::class_<Events::StartGame>("StartGame");
	python::class_<Events::Collided>("Collided")
		.def("getother", &Events::Collided::GetOther, python::return_internal_reference<>());
	python::class_<Events::TriggerEnter>("TriggerEnter")
		.def("entity", &Events::TriggerEnter::GetEntity, python::return_internal_reference<>());
	python::class_<Events::TriggerExit>("TriggerExit")
		.def("entity", &Events::TriggerExit::GetEntity, python::return_internal_reference<>());
	//python::class_<Events::RunnerDestroyed>("RunnerDestroyed")
	//	.def_readwrite("other", &Events::RunnerDestroyed::other)
	//	.def("getother", &Events::RunnerDestroyed::GetOther, python::return_internal_reference<>());
	python::class_<Events::Revived>("Revived");

}

std::weak_ptr<Entity> Create(Entity *parent) {
	return Entity::Create(parent);
}

BOOST_PYTHON_MODULE(entity) {
	python::class_<Entity, std::shared_ptr<Entity>>("Entity")
		// For every type of component we want to create in scripts, we need to
		// add an overload here
		.add_property("id", &Entity::Id)
		.def("get_parent", &Entity::GetParent, python::return_internal_reference<>())
		.def("add_component", AddComponent<Vehicle>)
		.def("add_component", AddComponent<Camera>)
		//.def("add_component", AddComponent<Runner>)
		.def("add_component", AddComponent<Image>)
		.def("add_component", AddComponent<Mesh>)
		.def("add_component", AddComponent<RigidBody>)
		.def("add_component", AddComponent<ScriptComponent>)
		.def("add_component", AddComponent<Trigger>)
		.def("add_component", AddComponent<Text>)
		.def("add_component", AddScriptComponent)
		.def("fire_event", &Entity::FireEvent<Events::Infected>)
		.def("fire_event", &Entity::FireEvent<Events::Destroyed>)
		.def("fire_event", &Entity::FireEvent<Events::Collided>)
		//.def("fire_event", &Entity::FireEvent<Events::RunnerDestroyed>)
		//.def("fire_event", &Entity::FireEvent<Events::RunnerCreated>)
		.def("fire_event", &Entity::FireEvent<Events::TriggerEnter>)
		.def("fire_event", &Entity::FireEvent<Events::TriggerExit>)
		.def("fire_event", &Entity::FireEvent<Events::Revived>)
//		.def("fire_event", &Entity::FireEvent<Events::StartGame>)
		.def("fire_event", FireScriptEvent)
		//.def("broadcast_event", &Entity::BroadcastEvent<Events::RunnerCreated>)
		.def("broadcast_event", &Entity::BroadcastEvent<Events::Infected>)
		.def("broadcast_event", &Entity::BroadcastEvent<Events::Destroyed>)
		.def("broadcast_event", &Entity::BroadcastEvent<Events::Collided>)
		.def("broadcast_event", &Entity::BroadcastEvent<Events::TriggerEnter>)
		.def("broadcast_event", &Entity::BroadcastEvent<Events::TriggerExit>)
		//.def("broadcast_event", &Entity::BroadcastEvent<Events::RunnerDestroyed>)
		.def("broadcast_event", &Entity::BroadcastEvent<Events::Revived>)
//		.def("broadcast_event", &Entity::BroadcastEvent<Events::StartGame>)
		.def("broadcast_event", BroadcastScriptEvent)
		//.def("register_runnercreated_handler", RegisterEventHandler<Events::RunnerCreated>)
		.def("register_infected_handler", RegisterEventHandler<Events::Infected>)
		.def("register_destroyed_handler", RegisterEventHandler<Events::Destroyed>)
		.def("register_collided_handler", RegisterEventHandler<Events::Collided>)
		.def("register_triggerenter_handler", RegisterEventHandler<Events::TriggerEnter>)
		.def("register_triggerexit_handler", RegisterEventHandler<Events::TriggerExit>)
		//.def("register_runnerdestroyed_handler", RegisterEventHandler<Events::RunnerDestroyed>)
		//.def("register_runnerdestroyed_handler", RegisterEventHandler<Events::Revived>)
//		.def("register_start_game_handler", RegisterEventHandler<Events::StartGame>)
		.def("register_handler", RegisterScriptEventHandler)
		.def("destroy", &Entity::Destroy)
		.def("transform", &Entity::GetTransform, python::return_internal_reference<>())
		.add_property("global_rotation", &Entity::GetGlobalRotation, &Entity::SetGlobalRotation)
		.add_property("global_position", &Entity::GetGlobalPosition, &Entity::SetGlobalPosition)
		.def("create", &Create)
		.staticmethod("create");

	python::class_<std::weak_ptr<Entity>>("EntityPtr")
		.def("lock", &std::weak_ptr<Entity>::lock);

	python::class_<std::weak_ptr<Component>>("ComponentPtr")
		.def("lock", &std::weak_ptr<Component>::lock);
}

BOOST_PYTHON_MODULE(component) {
	python::class_<Component, std::shared_ptr<Component>>("Component");

	python::class_<ModelShader, std::shared_ptr<ModelShader>>("ModelShader")
		.def(python::init<const char *>());
	python::class_<ImageShader, std::shared_ptr<ImageShader>>("ImageShader", python::init<const char *>());

	python::class_<ScriptComponent, python::bases<Component>, std::shared_ptr<ScriptComponent>>("ScriptComponent", python::init<std::string, Physics &>())
		.add_property("id", &Component::Id)
		.def("entity", &Component::entity, python::return_internal_reference<>())
		.def("physics", &ScriptComponent::GetPhysics, python::return_internal_reference<>());

	python::class_<Mesh, python::bases<Component>, std::shared_ptr<Mesh>>
		("Mesh", python::init<std::shared_ptr<Shader>, const char *, vec3, vec3, GLuint>())
		.def(python::init<std::shared_ptr<ImageShader>, const char *, vec3, vec3, GLuint>())
		.def(python::init<std::shared_ptr<ModelShader>, const char *, vec3, vec3, GLuint>());

	python::class_<RigidBody, python::bases<Component>, std::shared_ptr<RigidBody>>
		("RigidBody", python::init<Physics &, const char *, float, bool>());

	python::class_<Trigger, python::bases<Component>, std::shared_ptr<Trigger>>
		("Trigger", python::init<Physics &, PxGeometry &, PxTransform &>());
}

BOOST_PYTHON_MODULE(vehicle) {
	/*python::class_<PxVehicleDrive4WRawInputData>("Input")
		.def("set_acceleration", &PxVehicleDrive4WRawInputData::setDigitalAccel)
		.def("set_steer_left", &PxVehicleDrive4WRawInputData::setDigitalSteerLeft)
		.def("set_steer_right", &PxVehicleDrive4WRawInputData::setDigitalSteerRight)
		.def("set_brake", &PxVehicleDrive4WRawInputData::setDigitalBrake);*/

	python::class_<Vehicle, std::shared_ptr<Vehicle>, python::bases<Component>>
		("Vehicle", python::init<Physics &, std::shared_ptr<Controller>, Vehicle::Configuration &>())
		.def(python::init<Physics &, std::shared_ptr<aiController>, Vehicle::Configuration &>())
		.def("set_active", &Vehicle::SetActive)
		.def("set_friction", &Vehicle::SetFriction)
		.add_property("omega", &Vehicle::GetOmega, &Vehicle::SetOmega)
		.add_property("torque", &Vehicle::GetTorque, &Vehicle::SetTorque);

	python::class_<Vehicle::Configuration>("Configuration")
		.def_readwrite("max_speed", &Vehicle::Configuration::maxSpeed)
		.def_readwrite("position", &Vehicle::Configuration::position)
		.def_readwrite("rotation", &Vehicle::Configuration::rotation)
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
		.def_readwrite("max_speed", &Vehicle::Configuration::maxSpeed)
		.def_readwrite("max_omega", &Vehicle::Configuration::maxOmega);
}

BOOST_PYTHON_MODULE(camera) {
	python::class_<Camera, std::shared_ptr<Camera>, python::bases<Component>>("Camera", python::init<Physics &>());
}

ScriptComponent::~ScriptComponent()
{
}

void ScriptComponent::RegisterHandlers()
{
	self_.attr("entity") = python::ptr(entity_);
	self_.attr("physics") = python::ptr(&physics_);
	Call("start");
}

void ScriptComponent::Update(seconds dt)
{
	Call("update", dt.count());
}

void ScriptComponent::Destroy()
{
}

void ScriptComponent::InitPython()
{
	static bool initialized = false;

	if (!initialized) {
		try {
			Py_SetPythonHome("Python27/");
			Py_Initialize();

			initcomponent();
			initentity();
			initphysics();
			initvehicle();
			initcontroller();
			initaicontroller();
			initevents();
			initcamera();
			initnavmesh();
			//initrunner();
			initui();

			initialized = true;
		} catch (const python::error_already_set &) {
			PyErr_Print();
		}
	}
}

void ScriptComponent::InitScript(const std::string &type)
{
	try {
		std::string filename = Util::ScriptDirectory + type + ".py";

		auto module = python::borrowed(PyImport_AddModule("__main__"));
		auto handle = python::handle<>(module);
		main_ = python::object(handle);
		locals_ = python::dict(main_.attr("__dict__"));

		python::exec("import gc;", locals_, locals_);
		python::exec_file(filename.c_str(), locals_, locals_);

		//if (locals_.has_key("init")) {
		//	locals_["init"](python::ptr(this));
		//}
	} catch (const python::error_already_set &) {
		PyErr_Print();
	}
}

// Workaround for known MSVC bug
namespace boost {
	template <>
	Entity const volatile * get_pointer<class Entity const volatile>(
		class Entity const volatile *c
		) {
		return c;
	}

	template <>
	Camera const volatile * get_pointer<class Camera const volatile>(
		class Camera const volatile *c
		) {
		return c;
	}

	//template <>
	//Runner const volatile * get_pointer<class Runner const volatile>(
	//	class Runner const volatile *c
	//	) {
	//	return c;
	//}

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
	Controller const volatile * get_pointer<class Controller const volatile>(
		class Controller const volatile *c
		) {
		return c;
	}

	template <>
	aiController const volatile * get_pointer<class aiController const volatile>(
		class aiController const volatile *c
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
	NavMesh const volatile * get_pointer<class NavMesh const volatile>(
		class NavMesh const volatile *c
		) {
		return c;
	}

	template <>
	Mesh const volatile * get_pointer<class Mesh const volatile>(
		class Mesh const volatile *c
		) {
		return c;
	}

	template <>
	ImageShader const volatile * get_pointer<class ImageShader const volatile>(
		class ImageShader const volatile *c
		) {
		return c;
	}

	template <>
	ModelShader const volatile * get_pointer<class ModelShader const volatile>(
		class ModelShader const volatile *c
		) {
		return c;
	}

	template <>
	PxVehicleDrive4WRawInputData const volatile * get_pointer<class PxVehicleDrive4WRawInputData const volatile>(
		class PxVehicleDrive4WRawInputData const volatile *c
		) {
		return c;
	}

	template <>
	Image const volatile * get_pointer<class Image const volatile>(
		class Image const volatile *c
		) {
		return c;
	}

	template <>
	RigidBody const volatile * get_pointer<class RigidBody const volatile>(
		class RigidBody const volatile *c
		) {
		return c;
	}

	template <>
	Trigger const volatile * get_pointer<class Trigger const volatile>(
		class Trigger const volatile *c
		) {
		return c;
	}

	template <>
	Text const volatile * get_pointer<class Text const volatile>(
		class Text const volatile *c
		) {
		return c;
	}
}
