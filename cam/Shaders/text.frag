#version 410

uniform sampler2D tex;
uniform vec3 textcolour;

in vec2 v_UV;

out vec4 colour;

void main()
{
    colour = vec4(textcolour, texture(tex, v_UV).r);
    // colour = vec4(1.0, 1.0, 1.0, 1.0);
}