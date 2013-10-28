#version 420

layout (location = 0) out vec4 fColor;

in vec3 gTriDistance;
in vec3 gTriDistanceCtrl;

uniform vec3 DiffuseColor;

const vec3 WireframeColor = vec3(1, 1, 1);
const float add = 0.02;
const float threshold = 0.0025;

//higher scale will produce thiner lines
//TODO create a more steep function
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
	if (f >= 100 && result.x >= threshold) result.x += add;
	f = min(sizes.x, sizes.z);
	if (f >= 100 && result.y >= threshold) result.y += add;
	f = min(sizes.y, sizes.z);
	if (f >= 100 && result.z >= threshold) result.z += add;
	return result;
}

void main(void)
{
	vec3 color = DiffuseColor;

	int i;
	vec3 adjusted = adjust(gTriDistance, gTriDistanceCtrl);
	float d = min(adjusted.x, min(adjusted.y, adjusted.z));
	float scale = 30;

	d = amplify(d, scale, -0.5);
	color = d * color + (1 - d) * WireframeColor;

    fColor = vec4(color, 1.0);
}
