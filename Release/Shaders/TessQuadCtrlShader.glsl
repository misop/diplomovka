#version 410

layout(vertices = 4) out;

in float vTessLevel[];

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

	gl_out[gl_InvocationID].gl_Position = gl_in[gl_InvocationID].gl_Position;
}