#version 410

layout (location = 0) in vec3 Position;
layout (location = 1) in vec3 VertexNormal;
layout (location = 2) in float TessLevel;
layout (location = 3) in vec3 SQMNode;
layout (location = 4) in ivec2 Data;

out vec3 vVertexNormal;
out float vTessLevel;
out vec3 vNodePosition;
flat out int vNodeType;
flat out int vNodeID;
 
void main(void)
{
	vVertexNormal = VertexNormal;
	vTessLevel = TessLevel;
	vNodePosition = SQMNode;
	vNodeType = Data.x;
	vNodeID = Data.y;
	gl_Position = vec4(Position, 1.0);
}