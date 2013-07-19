#version 410
 
layout (location = 0) in  vec3 in_Position;
layout (location = 1) in  vec3 in_Color;
out vec3 ex_Color;

uniform mat4 MVPmatrix;
 
void main(void)
{
	gl_Position = vec4(in_Position, 1.0);
	gl_Position = MVPmatrix * gl_Position;
	gl_PointSize = 10;
	ex_Color = vec3(1.0, 1.0, 1.0);
	ex_Color = in_Color;
}