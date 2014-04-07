#version 430

#define POINT_LIGHTS 8

layout(location = 0) in vec4 vertex_eye;
layout(location = 1) in vec4 normal_eye;
layout(location = 2) in vec4 light_eye;
layout(location = 5) in vec2 uv;
layout(location = 6) in vec4 shadowCoord;
layout(location = 7) in vec4 point_eye[POINT_LIGHTS];

layout(location = 4) uniform vec4 Material;
layout(location = 11) uniform vec4 SunColor;
layout(location = 20) uniform vec4 PointColor[POINT_LIGHTS];
layout(location = 28) uniform vec4 Attenuation[POINT_LIGHTS];
layout(binding=0) uniform sampler2D DiffuseSampler;
layout(binding=4) uniform sampler2D ShadowSampler;
//layout(binding=4) uniform sampler2DShadow ShadowSampler;

const float zNear = 1.0;
const float zFar = 15000;

layout (location = 0) out vec4 fColor;

void main(void)
{	   
	vec4 diffuse_material = texture(DiffuseSampler, uv);
	vec4 color = vec4(0.0, 0.75, 0.75, 1);

	vec4 V = normalize(vertex_eye);
   	vec4 L = normalize(light_eye);
   	vec4 N = normalize(normal_eye);
   	vec4 H = normalize(L + V); 
	
	float diffuse = clamp(dot(L, N), 0.0, 1.0);
	
	//shadow bias
	float bias = Material.z;
	//shadow visibility
	float visibility = 1.0;
	vec4 shadowmap = texture(ShadowSampler, shadowCoord.xy);
	float depth = shadowmap.r;
	if (depth < shadowCoord.z - bias) {
		visibility = 0.3;
	}
	
   	float specular = 0;
	if (Material.y >= 1) specular = sign(diffuse) * pow(clamp(dot(H, N), 0.0, 1.0), Material.y);

	color = 0.2 * (vec4(0.4, 0.4, 0.4, 1.0)) * (diffuse_material);
	color += visibility * diffuse * diffuse_material * SunColor;
	color += visibility * specular * SunColor;	
	
	for (int i = 0; i < POINT_LIGHTS; i++) {
		L = point_eye[i];
		float dist = length(L);
		L = normalize(L);
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
	//fColor = vec4(shadowCoord.Z);
}