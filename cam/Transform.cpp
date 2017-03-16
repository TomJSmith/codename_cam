#include "Transform.h"

#include <glm/gtx/transform.hpp>

Transform::Transform() :
	position(0.0f, 0.0f, 0.0f),
	rotation(1.0f, 0.0f, 0.0f, 0.0f),
	scale(1.0f, 1.0f, 1.0f)
{}

Transform::Transform(vec3 p, quaternion r, vec3 s) :
	position(p),
	rotation(r),
	scale(s)
{}

mat4 Transform::Matrix() const
{
	// TODO I think this is wrong... should probably be TRS so translation doesn't
	// depend on how the object is rotated
	return glm::translate(position) * glm::toMat4(rotation) * glm::scale(scale);
}

vec3 Transform::Forward() const
{
	vec3 ret = Matrix() * vec4(0.0f, 0.0f, 1.0f, 0.0f);
	return glm::normalize(ret);
}

vec3 Transform::Up() const
{
	vec3 ret = Matrix() * vec4(0.0f, 1.0f, 0.0f, 0.0f);
	return glm::normalize(ret);
}

vec3 Transform::Right() const
{
	vec3 ret = Matrix() * vec4(1.0f, 0.0f, 0.0f, 0.0f);
	return glm::normalize(ret);
}

vec3 Transform::GlobalPosition() const
{
	vec3 ret = Matrix() * vec4(0.0f, 0.0f, 0.0f, 1.0f);
	return ret;
}