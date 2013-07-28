#version 410
 
layout (location = 0) in vec3 Position;
layout (location = 1) in float TessLevel;

out float vTessLevel;
 
void main(void)
{
	vTessLevel = TessLevel;
	gl_Position = vec4(Position, 1.0);
}