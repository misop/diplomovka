#version 420

layout(vertices = 3) out;

uniform float TessLevelInner;
uniform float TessLevelOuter;

void main()
{
    gl_TessLevelInner[0] = TessLevelInner;
    gl_TessLevelOuter[0] = TessLevelOuter;
    gl_TessLevelOuter[1] = TessLevelOuter;
    gl_TessLevelOuter[2] = TessLevelOuter;

	gl_out[gl_InvocationID].gl_Position = gl_in[gl_InvocationID].gl_Position;
}
