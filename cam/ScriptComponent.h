#pragma once

#include <boost/python.hpp>

#include <map>

#include "Component.h"
#include "Events.h"

class Physics;

class ScriptComponent : public Component {
 public:
	template <typename... Args>
	ScriptComponent(const std::string &type, Physics &physics, Args... args) :
		type_(type),
		physics_(physics)
	{
		InitPython();
		InitScript(type_);

		// TODO gross
		type_[0] = ::toupper(type_[0]);

		std::transform(type_.begin(), type_.end(), type_.begin(), [](char c) {
			static bool uppercase = false;
			if (uppercase) {
				uppercase = false;
				return static_cast<char>(::toupper(c));
			}

			if (c == ' ' || c == '_') uppercase = true;
			return c;
		});

		type_.erase(std::remove(type_.begin(), type_.end(), '_'), type_.end());

		if (locals_.has_key(type_)) {
			try {
				self_ = locals_[type_](args...);
			}
			catch (const python::error_already_set &) {
				PyErr_Print();
			}
		}
	}

	ScriptComponent(boost::python::object self, Physics &physics) : self_(self), physics_(physics) {}

	//ScriptComponent(const std::string &filename, Physics &physics);
	~ScriptComponent();

	virtual void Update(seconds dt) override;

	Physics &GetPhysics() const { return physics_; }

 private:
	boost::python::dict locals_;
	boost::python::object main_;
	boost::python::object self_;

	std::string type_;

	Physics &physics_;

	virtual void RegisterHandlers() override;
	void InitScript(const std::string &filename);

	template <typename... Args>
	void Call(const std::string &fn, Args... args)
	{
		try {
			if (!self_.is_none() && PyObject_HasAttrString(self_.ptr(), fn.c_str())) {
				self_.attr(fn.c_str())(args...);
			}
		}
		catch (const python::error_already_set &) {
			PyErr_Print();
		}
	}

	static void InitPython();

	std::function<void(Events::Collided)> handler_;
};

