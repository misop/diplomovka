#version 430

layout(location = 0) in vec4 vertex_eye;
layout(location = 1) in vec4 normal_eye;
layout(location = 2) in vec4 light_eye;
layout(location = 5) in vec2 uv;
layout(location = 6) in vec4 shadowCoord;

layout(location = 4) uniform vec4 Material;
layout(location = 13) uniform vec4 SunColor;
layout(binding=0) uniform sampler2D DiffuseSampler;
layout(binding=5) uniform sampler2D ToonShadingSampler;
layout(binding=4) uniform sampler2D ShadowSampler;

const float Toonines = 11;
const float Toonines_1 = 1.0/Toonines;

layout (location = 0) out vec4 fColor;

void main(void) {	   
	vec4 diffuse_material = texture(DiffuseSampler, uv);
	diffuse_material = floor(diffuse_material * Toonines) * Toonines_1;
	vec4 color = vec4(0.0, 0.75, 0.75, 1);

	vec4 V = normalize(vertex_eye);
   	vec4 L = normalize(light_eye);
   	vec4 N = normalize(normal_eye);
	
	//shadow bias
	float bias = Material.z;
	//shadow visibility
	float visibility = 1.0;
	vec4 shadowmap = texture(ShadowSampler, shadowCoord.xy);
	float depth = shadowmap.r;
	if (depth < shadowCoord.z - bias) {
		visibility = 0.3;
	}
	
	//float diffuse = clamp(dot(L, N), 0.0, visibility);
	float diffuse = clamp(dot(L, N), 0.0, 1.0);
	diffuse = texture(ToonShadingSampler, vec2(diffuse, 0));
	
	vec4 ambient = vec4(0.2, 0.2, 0.2, 1.0);

	color = 0.2 * (vec4(0.2, 0.2, 0.2, 1.0) + ambient) * (diffuse_material);
	color += visibility * diffuse * diffuse_material * SunColor;

	fColor = color;
}