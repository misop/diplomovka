#version 410
 
layout (location = 0) in vec3 Position;
layout (location = 1) in float TessLevel;
layout (location = 2) in vec3 SQMNode;
layout (location = 3) in vec3 Radius;

out vec3 vTest;
out float vTessLevel;
out vec3 vNodePosition;
out vec3 vNodeRadius;
 
void main(void)
{
	vTessLevel = TessLevel;
	vNodePosition = SQMNode;
	vNodeRadius = Radius;
	gl_Position = vec4(Position, 1.0);
}