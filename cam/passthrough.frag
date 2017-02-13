#version 410

in vec3 v_colour;
in vec3 N;
in vec3 L;
in vec3 V;

out vec4 colour;

void main() {

	float diff = max(0.0, dot(N,L));
	vec3 R = reflect(-L, N);

	float e = 100.0;
	float spec = pow(max(dot(R, V), 0.0), e);


	//vec3 diffuse = vec3(0.0f, 1.0f, 0.0f) * diff;
	//vec3 specular = vec3(1.0f, 1.0f, 1.0f) * spec;
	//vec3 ambient = vec3(1.0f, 1.0f, 1.0f) * 0.2f;

	vec3 diffuse = v_colour * diff;
	vec3 specular = v_colour * spec;
	vec3 ambient = v_colour * 0.2f;

	colour = vec4(ambient + diffuse + specular, 1.0f);

   // colour = vec4(v_colour, 1.0);
 
}