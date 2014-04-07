#version 430
 
layout(location = 0) in vec3 Position;
layout(location = 1) in vec3 Normal;

layout(location = 0) uniform mat4 ModelMatrix;
layout(location = 1) uniform mat4 ViewMatrix;
layout(location = 2) uniform mat4 ProjectionMatrix;
layout(location = 3) uniform mat3 NormalMatrix;

layout(location = 0) out vec3 normal_eye;
layout(location = 1) out vec4 pos_eye;
layout(location = 2) out float linear_depth;
 
void main(void)
{
	normal_eye = NormalMatrix * Normal;
	pos_eye = ViewMatrix * ModelMatrix * vec4(Position, 1.0);
	//linear_depth
	gl_Position = ProjectionMatrix * pos_eye;
}