#version 430

#define POINT_LIGHTS 8

layout (location = 0) in vec3 Position;
layout (location = 1) in vec3 Normal;
layout (location = 2) in vec2 UV;
layout (location = 3) in vec3 Tangent;
layout (location = 4) in vec3 Bitangent;

layout(location = 0) uniform mat4 ModelMatrix;
layout(location = 1) uniform mat4 ViewMatrix;
layout(location = 2) uniform mat4 ProjectionMatrix;
layout(location = 3) uniform mat3 NormalMatrix;
layout(location = 6) uniform mat4 ShadowMatrix;

layout(location = 10) uniform vec4 Sun;
layout(location = 12) uniform vec4 Point[POINT_LIGHTS];

const float zNear = 1800;
const float zFar = 6400;

layout(location = 0) out vec4 vertex_eye;
layout(location = 1) out vec4 normal_eye;
layout(location = 2) out vec4 light_eye;
layout(location = 3) out vec3 tangent_eye;
layout(location = 4) out vec3 bitangent_eye;
layout(location = 5) out vec2 uv;
layout(location = 6) out vec4 shadowCoord;
layout(location = 7) out vec4 point_eye[POINT_LIGHTS];

void main(void) {
	vec4 pos = vec4(Position, 1.0);
	vec4 normal = vec4(Normal, 0.0);

	//camera in eye coordinates is at (0,0,0,0) and light is at camera location now
	vertex_eye = ViewMatrix * ModelMatrix * pos;
	light_eye = -vec4(NormalMatrix*vec3(Sun), 0.0);//ViewMatrix*Sun - vertex_eye;//vec4(0, 0, 0, 1) - vertex_eye;
	for (int i = 0; i < POINT_LIGHTS; i++)
		point_eye[i] = ViewMatrix*Point[i] - vertex_eye;
	normal_eye = vec4(NormalMatrix * vec3(normal), 0.0);
	vertex_eye = vec4(0, 0, 0, 1) - vertex_eye;
	tangent_eye = NormalMatrix * Tangent;
	bitangent_eye = NormalMatrix * Bitangent;
	
	shadowCoord = ShadowMatrix * ModelMatrix * pos;
	shadowCoord /= shadowCoord.w;
	shadowCoord.xyz = shadowCoord.xyz*0.5 + 0.5;
	
	uv = UV;

	gl_Position = ProjectionMatrix * ViewMatrix * ModelMatrix * pos;
}