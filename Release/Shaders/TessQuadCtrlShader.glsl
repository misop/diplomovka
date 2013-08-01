#version 410

layout(vertices = 4) out;

in float vTessLevel[];
in vec3 vNodePosition[];
in float vNodeRadius[];

out vec3 tcNodePosition[];
out float tcNodeRadius[];

void main()
{
	float tessLevel = round((vTessLevel[0] + vTessLevel[1] + vTessLevel[2] + vTessLevel[3]) / 4.0);
	//inner level
    gl_TessLevelInner[0] = tessLevel;
    gl_TessLevelInner[1] = tessLevel;
	//outter levels
    gl_TessLevelOuter[0] = tessLevel;
    gl_TessLevelOuter[1] = tessLevel;
    gl_TessLevelOuter[2] = tessLevel;
    gl_TessLevelOuter[3] = tessLevel;
	
	tcNodePosition[gl_InvocationID] = vNodePosition[gl_InvocationID];
	tcNodeRadius[gl_InvocationID] = vNodeRadius[gl_InvocationID];

	gl_out[gl_InvocationID].gl_Position = gl_in[gl_InvocationID].gl_Position;
}