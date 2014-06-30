#version 420

layout(location = 0) in vec4 vertex_eye;
layout(location = 1) in vec4 normal_eye;
layout(location = 2) in vec4 light_eye;

layout (location = 0) out vec4 fColor;

uniform vec3 DiffuseColor;
 
void main(void)
{
	vec4 V = normalize(vertex_eye);
   	vec4 L = normalize(light_eye);
   	vec4 N = normalize(normal_eye);

	float diffuse = clamp(dot(L, N), 0.0, 1.0) + 0.1;

	fColor = vec4(DiffuseColor * diffuse, 1.0);
}