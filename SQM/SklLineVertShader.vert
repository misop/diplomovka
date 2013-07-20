#version 410
 
layout (location = 0) in  vec3 Position;

out vec3 vColor;

uniform mat4 MVPmatrix;
 
void main(void)
{
	gl_Position = MVPmatrix * vec4(Position, 1.0);
}