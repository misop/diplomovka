#version 430
 
layout(location = 0) in vec3 Position;
layout(location = 1) in vec3 Normal;

layout(location = 0) uniform mat4 ModelMatrix;
layout(location = 1) uniform mat4 ViewMatrix;
layout(location = 2) uniform mat4 ProjectionMatrix;
layout(location = 3) uniform mat3 NormalMatrix;

layout(location = 0) out vec3 normal_eye;
 
void main(void)
{
	normal_eye = NormalMatrix * Normal;
	gl_Position = ProjectionMatrix * ViewMatrix * ModelMatrix * vec4(Position, 1.0);
}