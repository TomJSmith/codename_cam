#version 410

layout(location = 0) in vec2 position;
layout(location = 1) in vec2 UV;

uniform mat3 transform;
uniform uint layer;
uniform uint max_layers;
out vec2 v_UV;

void main()
{
    vec2 transformed = vec2(transform * vec3(position, 1.0));
    gl_Position = vec4(transformed, float(layer) / float(max_layers), 1.0);
    v_UV = UV;
}
