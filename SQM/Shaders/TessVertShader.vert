#version 410
 
layout (location = 0) in vec3 Position;
layout (location = 1) in float TessLevel;
layout (location = 2) in vec4 SQMNode;

out float vTessLevel;
out vec3 vNodePosition;
out float vNodeRadius;
 
void main(void)
{
	vTessLevel = TessLevel;
	vNodePosition = vec3(SQMNode);
	vNodeRadius = SQMNode.w;
	gl_Position = vec4(Position, 1.0);
}