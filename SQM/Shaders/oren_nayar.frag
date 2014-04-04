#version 430

// http://ruh.li/GraphicsOrenNayar.html

layout(location = 0) in vec4 vertex_eye;
layout(location = 1) in vec4 normal_eye;
layout(location = 2) in vec4 light_eye;
layout(location = 5) in vec2 uv;

layout(location = 4) uniform vec4 Material;
layout(location = 13) uniform vec4 SunColor;
layout(binding=0) uniform sampler2D DiffuseSampler;
//layout(binding=4) uniform sampler2DShadow ShadowSampler;

const float roughness = 5;

layout (location = 0) out vec4 fColor;

void main(void)
{	   
	vec4 diffuse_material = texture(DiffuseSampler, uv);
	vec4 color = vec4(0.0, 0.75, 0.75, 1);

	vec4 V = normalize(vertex_eye);
   	vec4 L = normalize(light_eye);
   	vec4 N = normalize(normal_eye);
	
	// calculate intermediary values
	float NdotL = dot(N, L);
	float NdotV = dot(N, V); 

	float angleVN = acos(NdotV);
	float angleLN = acos(NdotL);

	float alpha = max(angleVN, angleLN);
	float beta = min(angleVN, angleLN);
	float gamma = dot(V - N * dot(V, N), L - N * dot(L, N));

	float roughnessSquared = roughness * roughness;
	
	// calculate A and B
	float A = 1.0 - 0.5 * (roughnessSquared / (roughnessSquared + 0.57));
	float B = 0.45 * (roughnessSquared / (roughnessSquared + 0.09));
	float C = sin(alpha) * tan(beta);
	
	// put it all together
	float L1 = max(0.0, NdotL) * (A + B * max(0.0, gamma) * C);
	
	// get the final color 
	fColor = L1 * diffuse_material * SunColor;
}