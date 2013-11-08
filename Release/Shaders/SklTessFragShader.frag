#version 420

layout (location = 0) out vec4 fColor;

uniform vec3 DiffuseColor;
 
void main(void)
{
	fColor = vec4(DiffuseColor, 1.0);
}