#version 430

layout(location = 0) in vec3 Position;

layout(location = 0) uniform mat4 ModelMatrix;
layout(location = 1) uniform mat4 ViewMatrix;
layout(location = 2) uniform mat4 ProjectionMatrix;

out vec3 position;

void main(void)
{
	vec4 pos = ProjectionMatrix * ViewMatrix * ModelMatrix * vec4(Position, 1.0);
	position = vec3(pos);
	gl_Position = pos;
}