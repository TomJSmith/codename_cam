#pragma once

#include <glm/gtx/quaternion.hpp>
#include <glm/vec3.hpp>
#include <glm/mat4x4.hpp>

using vec3 = glm::vec3;
using vec4 = glm::vec4;
using mat4 = glm::mat4;
using quaternion = glm::quat;

class Transform
{
public:
	Transform();
	Transform(vec3 p, quaternion r, vec3 s);

	mat4 Matrix() const;
	vec3 Forward() const;
	vec3 Up() const;
	vec3 Right() const;
	vec3 GlobalPosition() const;

	vec3 position;
	quaternion rotation;
	vec3 scale;
};

