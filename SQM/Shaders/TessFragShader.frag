#version 420

layout (location = 0) out vec4 fColor;

in vec3 gTriDistance;
in vec3 gTriDistanceCtrl;
in vec4 gPatchDistance;
in vec4 gPatchDistanceCtrl;
in vec3 gVertex;
in vec3 gNormal;
in vec3 gColor;

uniform vec3 LightPosition;
uniform vec3 DiffuseColor;
uniform vec4 AmbientColor;
uniform mat4 MVPmatrix;
uniform mat4 ModelViewMatrix;
uniform int Wireframe;

const vec3 WireframeColor = vec3(1, 1, 1);
const float add = 0.02;

const float lengthThreshold4 = 50;
const float threshold4 = 0.0025;

const float lengthThreshold3 = 30;
const float threshold3 = 0.0025;

float amplify(float d, float scale, float offset)
{
    d = scale * d + offset;
    d = clamp(d, 0, 1);
    d = 1 - exp2(-2*d*d);
    return d;
}

vec3 adjust(vec3 vals, vec3 sizes) {
	vec3 result = vals;
	float f = min(sizes.x, sizes.y);
	if (f >= lengthThreshold3 && result.x >= threshold3) result.x += add;
	f = min(sizes.x, sizes.z);
	if (f >= lengthThreshold3 && result.y >= threshold3) result.y += add;
	f = min(sizes.y, sizes.z);
	if (f >= lengthThreshold3 && result.z >= threshold3) result.z += add;
	return result;
}

vec4 adjust(vec4 vals, vec4 sizes) {
	vec4 result = vals;
	float f = min(sizes.w, sizes.y);
	if (f >= lengthThreshold4 && result.x >= threshold4) result.x += add;
	if (f >= lengthThreshold4 && result.z >= threshold4) result.z += add;

	f = min(sizes.x, sizes.z);
	if (f >= lengthThreshold4 && result.y >= threshold4) result.y += add;
	if (f >= lengthThreshold4 && result.w >= threshold4) result.w += add;

	return result;
}

void main()
{
    vec3 color = AmbientColor.w * vec3(AmbientColor);

	vec3 light = normalize(vec3(ModelViewMatrix * vec4(LightPosition, 1)) - gVertex);
	//float diffuseFactor = max(dot(gNormal, light), 0);
	float diffuseFactor = abs(dot(gNormal, light));
	color += diffuseFactor * DiffuseColor;
	//color = DiffuseColor;
	//color = gNormal;

	if (Wireframe == 1) {

		vec3 adjusted0 = adjust(gTriDistance, gTriDistanceCtrl);
		float d1 = min(min(adjusted0.x, adjusted0.y), adjusted0.z);
		//float d1 = min(min(gTriDistance.x, gTriDistance.y), gTriDistance.z);

		vec4 adjusted1 = adjust(gPatchDistance, gPatchDistanceCtrl);
		float d2 = min(min(min(adjusted1.x, adjusted1.y), adjusted1.z), adjusted1.w);
		//float d2 = min(min(min(gPatchDistance.x, gPatchDistance.y), gPatchDistance.z), gPatchDistance.w);
		d1 = 1 - amplify(d1, 50, -0.5);
		d2 = amplify(d2, 50, -0.5);
		color = d2 * color + d1 * d2 * WireframeColor;
	}
	
	//color = gColor;
    fColor = vec4(color, 1.0);
}
