#version 430

// http://ruh.li/GraphicsCookTorrance.html

layout(location = 0) in vec4 vertex_eye;
layout(location = 1) in vec4 normal_eye;
layout(location = 2) in vec4 light_eye;
layout(location = 5) in vec2 uv;
layout(location = 6) in vec4 shadowCoord;

layout(location = 4) uniform vec4 Material;
layout(location = 13) uniform vec4 SunColor;
layout(binding=0) uniform sampler2D DiffuseSampler;
layout(binding=4) uniform sampler2D ShadowSampler;
//layout(binding=4) uniform sampler2DShadow ShadowSampler;

const float roughnessValue = 0.3; // 0 : smooth, 1: rough
const float F0 = 0.8; // fresnel reflectance at normal incidence
const float k = 0.2; // fraction of diffuse reflection (specular reflection = 1 - k)

layout (location = 0) out vec4 fColor;

void main(void)
{	   
	vec4 diffuse_material = texture(DiffuseSampler, uv);
	vec4 color = vec4(0.0, 0.75, 0.75, 1);

	vec4 V = normalize(vertex_eye);
   	vec4 L = normalize(light_eye);
   	vec4 N = normalize(normal_eye);
   	vec4 H = normalize(L + V); 

	// do the lighting calculation for each fragment.
	float NdotL = max(dot(N, L), 0.0);

	float diffuse = clamp(dot(L, N), 0.0, 1.0);
	float specular = 0.0;
	if(NdotL > 0.0) {
		// calculate intermediary values
		float NdotH = max(dot(N, H), 0.0); 
		float NdotV = max(dot(N, V), 0.0); // note: this could also be NdotL, which is the same value
		float VdotH = max(dot(V, H), 0.0);
		float mSquared = roughnessValue * roughnessValue;

		// geometric attenuation
		float NH2 = 2.0 * NdotH;
		float g1 = (NH2 * NdotV) / VdotH;
		float g2 = (NH2 * NdotL) / VdotH;
		float geoAtt = min(1.0, min(g1, g2));

		// roughness (or: microfacet distribution function)
		// beckmann distribution function
		float r1 = 1.0 / ( 4.0 * mSquared * pow(NdotH, 4.0));
		float r2 = (NdotH * NdotH - 1.0) / (mSquared * NdotH * NdotH);
		float roughness = r1 * exp(r2);

		// fresnel
		// Schlick approximation
		float fresnel = pow(1.0 - VdotH, 5.0);
		fresnel *= (1.0 - F0);
		fresnel += F0;

		specular = (fresnel * geoAtt * roughness) / (NdotV * NdotL * 3.14);
	}

	vec3 finalValue = vec3(SunColor) * NdotL * (k + specular * (1.0 - k));
	fColor = 0.2 * (vec4(0.2, 0.2, 0.2, 1.0)) * (diffuse_material);
	fColor += diffuse * diffuse_material * SunColor;
	fColor += vec4(finalValue, 1.0);
}