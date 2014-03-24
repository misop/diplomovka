#version 430

layout(location = 0) in vec2 uvs;

layout(location = 1) uniform mat4 ProjectionMatrix_inv;
layout(location = 8) uniform float ScreenHeight;
layout(location = 9) uniform float ScreenWidth;
layout(binding=0) uniform sampler2D NoiseSamper;
layout(binding=1) uniform sampler2D ScreenSampler;
layout(binding=2) uniform sampler2D PositionSampler;

layout(location = 0) out vec4 fColor;

//noise texture size
float random_size = 64;
//the sampling radius
float g_sample_rad = 5;//5;//0.05;//0.1;//0.015;//0.05;//0.015;
//the ao intensity
float g_intensity = 5;
//scales distance between occluders and occludee
float g_scale = 0;//0.000005;//0.5;//5;//10;//50;
//controls the width of the occlusion cone considered by the occludee
float g_bias = 0.1;//0;//0.01;//0.05;

// Function for converting depth to view-space position
// in deferred pixel shader pass.  vTexCoord is a texture
// coordinate for a full-screen quad, such that x=0 is the
// left of the screen, and y=0 is the top of the screen.
vec3 PositionFromDepth(in vec2 vTexCoord) {
    // Get the depth value for this pixel
    float z = texture(ScreenSampler, vTexCoord).a;  
    // Get x/w and y/w from the viewport position
    float x = vTexCoord.x * 2 - 1;
    float y = (vTexCoord.y) * 2 - 1;
    vec4 vProjectedPos = vec4(x, y, z, 1.0f);
    // Transform by the inverse projection matrix
    vec4 vPositionVS = ProjectionMatrix_inv * vProjectedPos;  
    // Divide by w to get the view-space position
    return vPositionVS.xyz / vPositionVS.w;  
}

vec3 getPosition(in vec2 coord) {
	return texture(PositionSampler, coord).xyz;
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
	
	vec3 p = getPosition(uv);
	vec3 n = getNormal(uv);
	vec2 rand = getRandom(uv);

	float ao = 0.0f;
	float rad = g_sample_rad/p.z;

	//**SSAO Calculation**//
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
	if (ao <0.01) discard;
	fColor = vec4(0);
	fColor.a = (1 - ao);
}
