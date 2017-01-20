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
	return glm::toMat4(rotation) * glm::translate(position) * glm::scale(scale);
}