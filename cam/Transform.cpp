#include "Transform.h"

#include <glm/gtx/transform.hpp>

Transform::Transform() :
	position(0.0f, 0.0f, 0.0f),
	rotation(),
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

quaternion LookAt(const vec3 &origin, const vec3 &target, const vec3 &up)
{
	//return quaternion(glm::lookAt(origin, target, up));
	vec3 dir = glm::normalize(target - origin);
	float dot = glm::dot(vec3(0, 0, 1), dir);

	if (std::fabs(dot + 1.0f) < 0.0001f) {
		return glm::angleAxis(3.14159f, glm::vec3(0, 1, 0));
	}
	else if (std::fabs(dot - 1.0f) < 0.0001f) {
		return quaternion();
	}

	float angle = std::acosf(dot);
	vec3 cross = glm::normalize(glm::cross(vec3(0, 0, 1), dir));
	return glm::angleAxis(angle, cross);
}