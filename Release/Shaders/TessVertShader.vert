#version 420

#define SKINNING_MATRICES fill_in_skinnig_matrices_number

layout (location = 0) in vec3 Position;
layout (location = 1) in vec3 VertexNormal;
layout (location = 2) in float TessLevel;
layout (location = 3) in vec3 SQMNode;
layout (location = 4) in ivec2 SkinningIDs;
layout (location = 5) in ivec2 Data;

uniform mat4 SkinningMatrices[SKINNING_MATRICES];
uniform mat4 TransformMatrices[SKINNING_MATRICES];
uniform int SkinningType;

out vec3 vVertexNormal;
out float vTessLevel;
out vec3 vNodePosition;
flat out int vNodeType;
flat out int vNodeID;
flat out ivec2 vSkinningIDs;
 
void main(void)
{
	vTessLevel = TessLevel;
	vNodeType = Data.x;
	vNodeID = Data.y;
	vSkinningIDs = SkinningIDs;
	vec4 nodePosition = vec4(SQMNode, 1.0);
	vec4 normal = vec4(VertexNormal, 0.0);
	vec4 position = vec4(Position, 1.0);
	if (SkinningType == 1) {
		float w0 = 1, w1 = 0;
		if (SkinningIDs.y != -1) {
			w0 = 0.5;
			w1 = 0.5;
		}
		vec4 pos = position;
		vec4 npos = nodePosition;
		vec4 n = normal;
		if (SkinningIDs.x != -1) {
			pos = w0 * SkinningMatrices[SkinningIDs.x] * position;
			npos = SkinningMatrices[SkinningIDs.x] * nodePosition;
			n = w0 * SkinningMatrices[SkinningIDs.x] * normal;
		}
		if (SkinningIDs.y != -1) {
			pos += w1 * SkinningMatrices[SkinningIDs.y] * position;
			n += w1 * SkinningMatrices[SkinningIDs.y] * normal;
		}
		position = pos;
		nodePosition = npos;
		normal = normalize(n);
		if (SkinningIDs.x != -1) {
			position = TransformMatrices[SkinningIDs.x] * position;
			normal = normalize(TransformMatrices[SkinningIDs.x] * normal);
		}
	}

	vVertexNormal = vec3(normal);
	vNodePosition = vec3(nodePosition);
	gl_Position = position;
}