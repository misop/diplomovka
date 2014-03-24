#version 430

layout(location = 0) in vec3 Position;

layout(location = 0) uniform mat4 ModelMatrix;
layout(location = 1) uniform mat4 ViewMatrix;
layout(location = 2) uniform mat4 ProjectionMatrix;

void main(void)
{
	gl_Position =  ProjectionMatrix * ViewMatrix * ModelMatrix * vec4(Position, 1.0);
}