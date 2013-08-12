#version 410

layout (location = 0) in vec3 Position;
layout (location = 1) in float TessLevel;
layout (location = 2) in vec3 SQMNode;
layout (location = 3) in ivec2 Data;

out vec3 vTest;
out float vTessLevel;
out vec3 vNodePosition;
out float vNodeRadius;
out float vNodeType;
 
void main(void)
{
	vTessLevel = TessLevel;
	vNodePosition = SQMNode;
	vNodeRadius = 10.0;
	vNodeType = 1.0;
	gl_Position = vec4(Position, 1.0);
}