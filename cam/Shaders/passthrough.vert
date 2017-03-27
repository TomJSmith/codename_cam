#version 410

layout(location = 0) in vec3 position;
layout(location = 1) in vec3 colour;
layout(location = 2) in vec3 normal;
layout(location = 3) in vec2 texCoord;

uniform mat4 mvp;
uniform mat4 projection;
uniform mat4 modelview;

out vec2 TexCoord0;
out vec3 v_colour;
out vec3 N; //normal
out vec3 L; //light
out vec3 V; //view

void main() {
	
	mat3 normalMatrix = mat3(transpose(inverse(modelview)));
    vec3 Norm = normal;
	vec3 nor = normalMatrix * Norm;

	vec4 positionCameraSpace = modelview * vec4(position, 1.0f);
	vec3 P = positionCameraSpace.xyz / positionCameraSpace.w;
	
	L = normalize(vec3(0.0f, 10.0f, 0.0f) - P);
	N = normalize(nor);
	V = normalize(-P);

    gl_Position = mvp * vec4(position, 1.0f);
    v_colour = colour;
	TexCoord0 = texCoord;
}
