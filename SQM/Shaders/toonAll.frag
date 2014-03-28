#version 430

layout(location = 0) in vec4 vertex_eye;
layout(location = 1) in vec4 normal_eye;
layout(location = 2) in vec4 light_eye;
layout(location = 3) in vec3 tangent_eye;
layout(location = 4) in vec3 bitangent_eye;
layout(location = 5) in vec2 uv;

layout(location = 13) uniform vec4 SunColor;
layout(binding=0) uniform sampler2D DiffuseSampler;
layout(binding=2) uniform sampler2D NormalSampler;
layout(binding=5) uniform sampler2D ToonShadingSampler;

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
   	vec3 T = normalize(tangent_eye); 
   	vec3 B = normalize(bitangent_eye); 
	
	mat3 TBN = transpose(mat3(T, B, vec3(N)));
	V = vec4(TBN * vec3(V), 0.0);
	L = vec4(TBN * vec3(L), 0.0);
	N = normalize(texture2D(NormalSampler, uv)*2.0 - 1.0);
	

	float diffuse = clamp(dot(L, N), 0.0, 1.0);
	diffuse = texture(ToonShadingSampler, vec2(diffuse, 0));
	
	vec4 ambient = vec4(0.2, 0.2, 0.2, 1.0);

	color = 0.2 * (vec4(0.2, 0.2, 0.2, 1.0) + ambient) * (diffuse_material);
	color += diffuse * diffuse_material * SunColor;

	fColor = color;
}