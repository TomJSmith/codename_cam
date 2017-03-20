#version 410

uniform sampler2D tex;

in vec2 v_UV;

out vec4 colour;

void main()
{
    colour = texture(tex, v_UV);
    /* colour = vec4(1.0, 1.0, 1.0, 1.0); */
}
