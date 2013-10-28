#version 420
 
layout (location = 0) in  vec3 Position;

uniform mat4 MVPmatrix;
 
void main(void)
{
	gl_Position = MVPmatrix * vec4(Position, 1.0);
}