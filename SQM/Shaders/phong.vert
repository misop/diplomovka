#version 430

#define SKINNING_MATRICES fill_in_skinnig_matrices_number

layout (location = 0) in vec3 Position;
layout (location = 1) in vec3 Normal;
layout (location = 2) in ivec4 id;
//layout (location = 2) in ivec2 id;
layout (location = 3) in vec4 w;

layout(location = 0) uniform mat4 ModelMatrix;
layout(location = 1) uniform mat4 ViewMatrix;
layout(location = 2) uniform mat4 ProjectionMatrix;
layout(location = 3) uniform mat3 NormalMatrix;
layout(location = 4) uniform mat4 SkinningMatrices[SKINNING_MATRICES];
uniform mat4 SkinningMatricesReference[SKINNING_MATRICES];
uniform float AnimInterpolation;

out vec4 vertex_eye;
out vec4 normal_eye;
out vec4 light_eye;

void main(void) {
	vec4 pos = vec4(Position, 1.0);
	vec4 normal = vec4(Normal, 0.0);
	
	//skinning
	mat4 M1 = (w.x*SkinningMatricesReference[id.x] + w.y*SkinningMatricesReference[id.y] + w.z*SkinningMatricesReference[id.z]+ w.w*SkinningMatricesReference[id.w]);
	mat4 M2 = (w.x*SkinningMatrices[id.x] + w.y*SkinningMatrices[id.y] + w.z*SkinningMatrices[id.z]+ w.w*SkinningMatrices[id.w]);
	mat4 M = (1-AnimInterpolation)*M1 + AnimInterpolation*M2;
	pos = M * pos;
	normal = M * normal;

	//camera in eye coordinates is at (0,0,0,0) and light is at camera location now
	vertex_eye = ViewMatrix * ModelMatrix * pos;
	light_eye = vec4(0, 0, 0, 1) - vertex_eye;
	normal_eye = vec4(NormalMatrix * vec3(normal), 0.0);
	vertex_eye = vec4(0, 0, 0, 1) - vertex_eye;

	gl_Position = ProjectionMatrix * ViewMatrix * ModelMatrix * pos;
}