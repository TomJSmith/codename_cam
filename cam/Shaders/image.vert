#version 410

layout(location = 0) in vec2 position;
layout(location = 1) in vec2 UV;

uniform mat3 transform;
out vec2 v_UV;

void main()
{
    gl_Position = vec4(transform * vec3(position, -1.0), 1.0);
    v_UV = UV;
}
