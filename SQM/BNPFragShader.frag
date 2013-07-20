#version 410

layout (location = 0) out vec4 fColor;

in vec3 gTriDistance;

const vec3 WireframeColor = vec3(1, 1, 1);

//higher scale will produce thiner lines
//TODO create a more steep function
float amplify(float d, float scale, float offset)
{
    d = scale * d + offset;
    d = clamp(d, 0, 1);
    d = 1 - exp2(-50*d*d);
    return d;
}

void main(void)
{
	vec3 color = vec3(0.0, 0.75, 0.75);
	float d = min(min(gTriDistance.x, gTriDistance.y), gTriDistance.z);
	d = amplify(d, 30, -0.5);
	color = d * color + (1 - d) * WireframeColor;

    fColor = vec4(color, 1.0);
}
