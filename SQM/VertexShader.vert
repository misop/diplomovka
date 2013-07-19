#version 410
 
layout (location = 0) in  vec3 Position;
layout (location = 1) in  vec3 Color;

out vec3 vColor;

uniform mat4 MVPmatrix;
uniform mat4 ModelMatrix;
uniform int SelectedNode;
 
void main(void)
{
	gl_Position = vec4(Position, 1.0);
	if (SelectedNode == 0) {
		vColor = Color;
	} else {
		vColor = vec3(1.0, 1.0, 0.0);
	}
}