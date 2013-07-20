#version 410
 
layout (location = 0) in  vec3 Position;
layout (location = 1) in  vec3 Color;

out vec3 vColor;

uniform mat4 MVPmatrix;
uniform mat4 ModelMatrix;
uniform int SelectedNode;
uniform int CameraPos;
 
void main(void)
{
	gl_Position = vec4(Position, 1.0);
	if (SelectedNode == 1) vColor = vec3(1.0, 1.0, 0.0);
	else if (CameraPos == 1) vColor = vec3(0.9, 0.45, 0.0);
	else vColor = Color;
}