#version 430

#define POINT_LIGHTS 8

layout(location = 0) in vec4 vertex_eye;
layout(location = 1) in vec4 normal_eye;
layout(location = 2) in vec4 light_eye;
layout(location = 3) in vec3 tangent_eye;
layout(location = 4) in vec3 bitangent_eye;
layout(location = 5) in vec2 uv;
layout(location = 7) in vec4 shadowCoord;
layout(location = 8) in vec4 point_eye[POINT_LIGHTS];

layout(location = 4) uniform vec4 Material;
layout(location = 11) uniform vec4 SunColor;
layout(location = 20) uniform vec4 PointColor[POINT_LIGHTS];
layout(location = 28) uniform vec4 Attenuation[POINT_LIGHTS];
layout(binding=0) uniform sampler2D DiffuseSampler;
layout(binding=2) uniform sampler2D NormalSampler;
layout(binding=4) uniform sampler2D ShadowSampler;

layout (location = 0) out vec4 fColor;

void main(void) {	   
	vec4 diffuse_material = texture(DiffuseSampler, uv);
	vec4 color = vec4(0.0, 0.75, 0.75, 1);

	vec4 V = normalize(vertex_eye);
   	vec4 L = normalize(light_eye);
   	vec4 N = normalize(normal_eye); 
   	vec3 T = normalize(tangent_eye); 
   	vec3 B = normalize(bitangent_eye); 
	
	mat3 TBN = transpose(mat3(T, B, vec3(N)));
	V = vec4(TBN * vec3(V), 0.0);
	L = vec4(TBN * vec3(L), 0.0);
   	vec4 H = normalize(L + V); 
	N = normalize(texture2D(NormalSampler, uv)*2.0 - 1.0);
	
	//shadow bias
	float bias = Material.z;
	//shadow visibility
	float visibility = 1.0;
	vec4 shadowmap = texture(ShadowSampler, shadowCoord.xy);
	float depth = shadowmap.r;
	if (depth < shadowCoord.z - bias) {
		visibility = 0.3;
	}
	
	float diffuse = clamp(dot(L, N), 0.0, 1.0);
   	vec4 R = reflect(-L, N);
   	float specular = 0;
	if (Material.y >= 1) specular = sign(diffuse) * pow(clamp(dot(H, N), 0.0, 1.0), Material.y);

	color = 0.2 * vec4(0.4, 0.4, 0.4, 1.0) * (diffuse_material);
	color += visibility * diffuse * diffuse_material * SunColor;
	color += visibility * specular * SunColor;
	
	for (int i = 0; i < POINT_LIGHTS; i++) {
		L = point_eye[i];
		float dist = length(L);
		L = normalize(L);
		L = vec4(TBN * vec3(L), 0.0);
		H = normalize(L + V);
		diffuse = clamp(dot(L, N), 0.0, 1.0);
		specular = 0.0;
		if (Material.y >= 1.0) specular = sign(diffuse) * pow(clamp(dot(H, N), 0.0, 1.0), Material.y);
		
		float attenuation = Attenuation[i].x + Attenuation[i].y * dist + Attenuation[i].z * dist * dist;
		
		vec4 pcolor = diffuse * diffuse_material * PointColor[i];
		pcolor += specular * PointColor[i];	
		pcolor = pcolor * Attenuation[i].w / attenuation;
		color += pcolor;
	}

	fColor = color;
}