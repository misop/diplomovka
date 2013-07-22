#version 410

layout (location = 0) out vec4 fColor;

in vec3 gTriDistance;
in vec3 gTriDistanceCtrl;

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

float min3(float a, float b, float c, out int i) {
	if (a < c && b < c) {
		i = 0;
		return min(a, b);
	} else if (a < b && c < b) {
		i = 1;
		return min(a, c);
	}
	//if a was the smallest one of the ifs would trigger so a cannot be the smallest

	i = 2;
	return min(b, c);
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
	vec3 color = vec3(0.0, 0.75, 0.75);
	//float d = min(min(gTriDistance.x, gTriDistance.y), gTriDistance.z);
	//float ctrl = min(min(gTriDistanceCtrl.x, gTriDistanceCtrl.y), gTriDistanceCtrl.z);
	int i;
	vec3 adjusted = adjust(gTriDistance, gTriDistanceCtrl);
	float d = min3(adjusted.x, adjusted.y, adjusted.z, i);
	//float d = min3(gTriDistance.x, gTriDistance.y, gTriDistance.z, i);
	//float dist = gTriDistanceCtrl[i];
	//float dist = 0;
	//if (i == 0) dist = min(gTriDistanceCtrl.x, gTriDistanceCtrl.y);
	//if (i == 1) dist = min(gTriDistanceCtrl.x, gTriDistanceCtrl.z);
	//if (i == 2) dist = min(gTriDistanceCtrl.y, gTriDistanceCtrl.z);
	float scale = 30;
	//if (dist >= 100)
	//	scale += 50;

	d = amplify(d, scale, -0.5);
	color = d * color + (1 - d) * WireframeColor;

    fColor = vec4(color, 1.0);
}
