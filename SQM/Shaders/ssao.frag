#version 430

layout(location = 0) in vec2 uv;


layout(binding = 0) uniform sampler2D RandomSampler;
layout(binding = 1) uniform sampler2D ScreenSampler;
layout(binding = 2) uniform sampler2D PositionSampler;
layout(location = 1) uniform mat4 ProjectionMatrix_INV;
layout(location = 2) uniform mat4 ProjectionMatrix;

const float zNear = 1.0;
const float zFar = 15000;
const float uRadius = 10;
const float uBias = 30;

#define SAMPLES 16

layout(location = 0) out vec4 fColor;

/*const vec3 sampleKernel[8] = vec3[8](
    vec3(-0.011331562, -0.074309729, 0.033523623),
    vec3(-0.016038071, -0.073256440, 0),
    vec3(0.031890523, -0.050860822, 0.0052353325),
    vec3(0.016236259, 0.027160129, 0.0068024951),
    vec3(-0.30093661, -0.10254280, 0.0061832811),
    vec3(0.064812385, -0.19595113, 0.20066050),
    vec3(-0.032532435, -0.22817777, 0.057787783),
    vec3(0.31232032, 0.57163316, 0.023146300)
);*/

/*const vec3 sampleKernel[10] = vec3[](
	vec3(-0.010735935, 0.01647018, 0.0062425877),
	vec3(-0.06533369, 0.3647007, -0.13746321),
	vec3(-0.6539235, -0.016726388, -0.53000957),
	vec3(0.40958285, 0.0052428036, -0.5591124),
	vec3(-0.1465366, 0.09899267, 0.15571679),
	vec3(-0.44122112, -0.5458797, 0.04912532),
	vec3(0.03755566, -0.10961345, -0.33040273),
	vec3(0.019100213, 0.29652783, 0.066237666),
	vec3(0.8765323, 0.011236004, 0.28265962),
	vec3(0.29264435, -0.40794238, 0.15964167)
);*/

/*const vec3 sampleKernel[12] = vec3[](
	vec3(-0.13657719, 0.30651027, 0.16118456),
	vec3(-0.14714938, 0.33245975, -0.113095455),
	vec3(0.030659059, 0.27887347, -0.7332209),
	vec3(0.009913514, -0.89884496, 0.07381549),
	vec3(0.040318526, 0.40091, 0.6847858),
	vec3(0.22311053, -0.3039437, -0.19340435),
	vec3(0.36235332, 0.21894878, -0.05407306),
	vec3(-0.15198798, -0.38409665, -0.46785462),
	vec3(-0.013492276, -0.5345803, 0.11307949),
	vec3(-0.4972847, 0.037064247, -0.4381323),
	vec3(-0.024175806, -0.008928787, 0.17719103),
	vec3(0.694014, -0.122672155, 0.33098832)
);*/

const vec3 sampleKernel[16] = vec3[](
	vec3(0.53812504, 0.18565957, -0.43192),
	vec3(0.13790712, 0.24864247, 0.44301823),
	vec3(0.33715037, 0.56794053, -0.005789503),
	vec3(-0.6999805, -0.04511441, -0.0019965635),
	vec3(0.06896307, -0.15983082, -0.85477847),
	vec3(0.056099437, 0.006954967, -0.1843352),
	vec3(-0.014653638, 0.14027752, 0.0762037),
	vec3(0.010019933, -0.1924225, -0.034443386),
	vec3(-0.35775623, -0.5301969, -0.43581226),
	vec3(-0.3169221, 0.106360726, 0.015860917),
	vec3(0.010350345, -0.58698344, 0.0046293875),
	vec3(-0.08972908, -0.49408212, 0.3287904),
	vec3(0.7119986, -0.0154690035, -0.09183723),
	vec3(-0.053382345, 0.059675813, -0.5411899),
	vec3(0.035267662, -0.063188605, 0.54602677),
	vec3(-0.47761092, 0.2847911, -0.0271716)
);

void main(void)
{
	vec4 normal_depth = texture(ScreenSampler, uv);
	float normalized_depth = normal_depth.a;//unpack_depth(texture2D(depth_map, vTexCoord));

	// reconstruct eye-space position of fragment from linear depth
	/*float T1 = ProjectionMatrix[2][2];
	float T2 = ProjectionMatrix[2][3];
	float E1 = ProjectionMatrix[3][2];
	float linear_depth = -(zNear + (zFar - zNear) * normalized_depth);
	vec4 pos_clip;
	pos_clip.w = E1 * linear_depth;
	pos_clip.x = pos_clip.w * (2.0 * uv.x - 1.0);
	pos_clip.y = pos_clip.w * (2.0 * uv.y - 1.0);
	pos_clip.z = (T1 * linear_depth + T2);
	vec4 pos_eye = ProjectionMatrix_INV * pos_clip;*/
	vec4 pos_eye = texture(PositionSampler, uv);

	// get TBN matrix for transforming samples from TBN to eye-space
	vec3 normal = normal_depth.xyz * 2.0 - 1.0;
	normal = normalize(normal);
	vec3 randomVec = texture(RandomSampler, 100 * uv).xyz * 2.0 - 1.0;
	randomVec = normalize(randomVec);
	//vec3 tangent = normalize(randomVec - normal * dot(randomVec, normal));
	//vec3 bitangent = cross(normal, tangent);
	//mat3 TBN = mat3(tangent, bitangent, normal);

	float radius = uRadius;// * (1 - normalized_depth);

	float occlusion = 0.0;
	for (int i = 0; i < SAMPLES; i++) {
		// get sample position in eye space
		//vec3 sample_eye = TBN * sampleKernel[i];
		vec3 sample_eye = sampleKernel[i];//TBN * sampleKernel[i];
		sample_eye = reflect(sample_eye, randomVec);
		sample_eye = sign(dot(sample_eye, normal)) * sample_eye;
		sample_eye = sample_eye * radius + pos_eye.xyz;

		// project sample position to NDC 
		vec4 sample_ndc = ProjectionMatrix * vec4(sample_eye, 1.0);

		// get depth of sample from depth texture
		vec4 o_sample = texture(ScreenSampler, 0.5 * (sample_ndc.xy / sample_ndc.w) + 0.5);
		float sampleDepth = o_sample.a;//texture(ScreenSampler, 0.5 * (sample_ndc.xy / sample_ndc.w) + 0.5).a;
		sampleDepth = -(zNear + (zFar - zNear) * sampleDepth);

		// range check & accumulate
		float rangeCheck = (abs(pos_eye.z - sampleDepth)) < radius + uBias ? 1.0 : 0.0;
		occlusion += (sampleDepth <= sample_eye.z ? 1.0 : 0.0) * rangeCheck;
	}
	fColor = vec4(occlusion / float(SAMPLES));
}

/*//layout(location = 1) uniform mat4 ProjectionMatrix_inv;
layout(location = 8) uniform float ScreenHeight;
layout(location = 9) uniform float ScreenWidth;
layout(binding=0) uniform sampler2D NoiseSamper;
layout(binding=1) uniform sampler2D ScreenSampler;
layout(binding=2) uniform sampler2D PositionSampler;

layout(location = 0) out vec4 fColor;

//noise texture size
float random_size = 64;
//the sampling radius
float g_sample_rad = 10;
//the ao intensity
float g_intensity = 4;
//scales distance between occluders and occludee
float g_scale = 0.003;//0.000005;//0.5;//5;//10;//50;
//controls the width of the occlusion cone considered by the occludee
float g_bias = 0.01;//0.01;//0;//0.01;//0.05;

vec3 getPosition(in vec2 coord) {
	return texture(PositionSampler, coord).xyz;
	//return PositionFromDepth(coord);
}

vec4 getPositionAndDepth(in vec2 coord) {
	return texture(PositionSampler, coord);
	//return PositionFromDepth(coord);
}

vec3 getNormal(in vec2 uv) {
	return normalize(texture(ScreenSampler, uv).xyz * 2.0f - 1.0f);
}

vec2 getRandom(in vec2 coord) {
	return normalize(texture(NoiseSamper, vec2(ScreenWidth, ScreenHeight) * coord / random_size).xy * 2.0f - 1.0f);
}

float doAmbientOcclusion(in vec2 tcoord, in vec2 coord, in vec3 p, in vec3 cnorm) {
	vec3 diff = getPosition(tcoord + coord) - p;
	vec3 v = normalize(diff);
	float d = length(diff)*g_scale;
	return max(0.0, dot(cnorm, v)-g_bias)*(1.0/(1.0+d))*g_intensity;
}

void main(void)
{
	vec2 vec[4] = {vec2(1,0),vec2(-1,0), vec2(0,1),vec2(0,-1)};
	
	vec2 uv = uvs;
	//uv.x -= 5/ScreenWidth;
	
	vec4 point = getPositionAndDepth(uv);
	vec3 p = point.xyz;
	vec3 n = getNormal(uv);
	vec2 rand = getRandom(uv);

	float ao = 0.0f;
	float rad = g_sample_rad/p.z;

	//SSAO Calculation
	int iterations = 4;
	for (int j = 0; j < iterations; ++j) {
		  vec2 coord1 = reflect(vec[j],rand)*rad;
		  vec2 coord2 = vec2(coord1.x*0.707 - coord1.y*0.707, coord1.x*0.707 + coord1.y*0.707);
		  
		  ao += doAmbientOcclusion(uv,coord1*0.25, p, n);
		  ao += doAmbientOcclusion(uv,coord2*0.5, p, n);
		  ao += doAmbientOcclusion(uv,coord1*0.75, p, n);
		  ao += doAmbientOcclusion(uv,coord2, p, n);
	}
	ao/=float(iterations)*4.0;

	//Do stuff here with your occlusion value: modulate ambient lighting, write it to a buffer for later //use, etc.
	if (ao <0.1) discard;
	fColor = vec4(ao);
	fColor.a = point.a;//(1 - ao);
	fColor = clamp(fColor, 0.0, 1.0);
}*/
