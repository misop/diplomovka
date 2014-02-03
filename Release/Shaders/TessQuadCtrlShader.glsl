#version 420

layout(vertices = 4) out;

in vec3 vVertexNormal[];
in float vTessLevel[];
in vec3 vNodePosition[];
flat in int vNodeType[];
flat in int vNodeID[];
flat in ivec2 vSkinningIDs[];

uniform float TessLevelInner;

out vec3 tcVertexNormal[];
out vec3 tcNodePosition[];
flat out int tcNodeType[];
flat out int tcNodeID[];
flat out ivec2 tcSkinningIDs[];

void main()
{
	float tessLevel = round((vTessLevel[0] + vTessLevel[1] + vTessLevel[2] + vTessLevel[3]) / 4.0);
	if (TessLevelInner > 0.5) tessLevel = TessLevelInner;
	//inner level
    gl_TessLevelInner[0] = tessLevel;
    gl_TessLevelInner[1] = tessLevel;
	//outter levels
    gl_TessLevelOuter[0] = tessLevel;
    gl_TessLevelOuter[1] = tessLevel;
    gl_TessLevelOuter[2] = tessLevel;
    gl_TessLevelOuter[3] = tessLevel;
	
	tcVertexNormal[gl_InvocationID] = vVertexNormal[gl_InvocationID];
	tcNodePosition[gl_InvocationID] = vNodePosition[gl_InvocationID];
	tcNodeType[gl_InvocationID] = vNodeType[gl_InvocationID];
	tcNodeID[gl_InvocationID] = vNodeID[gl_InvocationID];
	tcSkinningIDs[gl_InvocationID] = vSkinningIDs[gl_InvocationID];

	gl_out[gl_InvocationID].gl_Position = gl_in[gl_InvocationID].gl_Position;
}