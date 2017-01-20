#version 410

layout(location = 0) in vec3 position;
layout(location = 1) in vec3 colour;

uniform mat4 mvp;

out vec3 v_colour;

void main() {
     gl_Position = mvp * vec4(position, 1.0f);
     v_colour = colour;
}