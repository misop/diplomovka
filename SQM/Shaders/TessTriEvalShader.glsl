#version 420

#define SKINNING_MATRICES fill_in_skinnig_matrices_number

layout(triangles, equal_spacing, cw) in;

flat in ivec2 tcSkinningIDs[];

out vec4 tePatchDistance;
out vec4 tePatchDistanceCtrl;
out vec3 tePos;

uniform mat4 MVPmatrix;
uniform int SkinningType;

uniform mat4 SkinningMatrices[SKINNING_MATRICES];
uniform mat4 TransformMatrices[SKINNING_MATRICES];

const float EPSILON = 0.00001;

bool floatEqual(in float a, in float b) {
	return abs(a - b) < EPSILON;
}

vec3 skinning(in vec3 position, in float v, in ivec2 tcSkinningID0, in ivec2 tcSkinningID1, in mat4 SkinningMatrices[SKINNING_MATRICES], in mat4 TransformMatrices[SKINNING_MATRICES], in int Type) {
	float w0 = (1-v), w1 = v;
	vec4 pos = vec4(position, 1.0f);
	vec4 pos0 = pos, pos1 = pos;
	ivec2 ids = ivec2(tcSkinningID0.x, tcSkinningID1.x);
	vec4 result = pos;
	//just forward
	if (Type == 2) {
		if (floatEqual(v, 0)) {
			w0 = 1;
			w1 = 0;
			ids = tcSkinningID0;
			if (ids.x != -1) {
				pos0 = SkinningMatrices[ids.x] * pos;
			}
			if (ids.y != -1) {
				w0 = 0.5;
				w1 = 0.5;
				pos1 = SkinningMatrices[ids.y] * pos;
			}
			result = w0*pos0 + w1*pos1;
		} else if (floatEqual(v, 1)) {
			w0 = 1;
			w1 = 0;
			ids = tcSkinningID1;
			if (ids.x != -1) {
				pos0 = SkinningMatrices[ids.x] * pos;
			}
			if (ids.y != -1) {
				w0 = 0.5;
				w1 = 0.5;
				pos1 = SkinningMatrices[ids.y] * pos;
			}
			result = w0*pos0 + w1*pos1;
		} else {
			ids = tcSkinningID1;
			if (ids.x != -1) {
				result = SkinningMatrices[ids.x] * pos;
			}
		}
	}
	//all avarage
	if (Type == 3) {
		w0 = 1; w1 = 0;
		if (tcSkinningID0.x != -1) {
			pos0 = SkinningMatrices[tcSkinningID0.x] * pos;
		}
		if (tcSkinningID0.y != -1) {
			w0 = 0.5;
			w1 = 0.5;
			pos1 = SkinningMatrices[tcSkinningID0.y] * pos;
		}
		vec4 t1 = w0*pos0 + w1*pos1;

		w0 = 1; w1 = 0;
		if (tcSkinningID1.x != -1) {
			pos0 = SkinningMatrices[tcSkinningID1.x] * pos;
		}
		if (tcSkinningID1.y != -1) {
			w0 = 0.5;
			w1 = 0.5;
			pos1 = SkinningMatrices[tcSkinningID1.y] * pos;
		}
		vec4 t2 = w0*pos0 + w1*pos1;

		result = (1-v)*t1 + (v)*t2;
	}

	return vec3(result);
}

void main()
{
	float maxi = max(max(gl_TessCoord.x, gl_TessCoord.y), gl_TessCoord.z);
    tePatchDistance = vec4(gl_TessCoord, maxi);
    float u = gl_TessCoord.x, v = gl_TessCoord.y;

    vec3 p0 = gl_TessCoord.x * gl_in[0].gl_Position.xyz;
    vec3 p1 = gl_TessCoord.y * gl_in[1].gl_Position.xyz;
    vec3 p2 = gl_TessCoord.z * gl_in[2].gl_Position.xyz;
	vec3 position = (p0 + p1 + p2);

	float d01 = length(gl_in[0].gl_Position - gl_in[1].gl_Position);
	float d12 = length(gl_in[1].gl_Position - gl_in[2].gl_Position);
	float d23 = length(gl_in[2].gl_Position - gl_in[3].gl_Position);
	float d30 = max(max(d01, d12), d23);
	tePatchDistanceCtrl = vec4(d01, d12, d23, d30);

	if (SkinningType >= 2) {
		position = skinning(position, v, tcSkinningIDs[0], tcSkinningIDs[1], SkinningMatrices, TransformMatrices, SkinningType);
	}
	
	//gl_Position = MVPmatrix * vec4(gl_TessCoord, 1);
	gl_Position = MVPmatrix * vec4(position, 1);
	tePos = position;
}
