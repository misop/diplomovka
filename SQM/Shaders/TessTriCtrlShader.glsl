#version 420

layout(vertices = 3) out;

in float vTessLevel[];
flat in ivec2 vSkinningIDs[];

flat out ivec2 tcSkinningIDs[];

void main()
{
	float tessLevel = round((vTessLevel[0] + vTessLevel[1] + vTessLevel[2]) / 3.0);
	tessLevel = 1;
	//inner level
    gl_TessLevelInner[0] = tessLevel;
	//outter levels
    gl_TessLevelOuter[0] = tessLevel;
    gl_TessLevelOuter[1] = tessLevel;
    gl_TessLevelOuter[2] = tessLevel;
	tcSkinningIDs[gl_InvocationID] = vSkinningIDs[gl_InvocationID];

	gl_out[gl_InvocationID].gl_Position = gl_in[gl_InvocationID].gl_Position;
}