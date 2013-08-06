#version 410

layout(quads) in;

in vec3 tcNodePosition[];
in float tcNodeRadius[];

out vec4 tePatchDistance;
out vec4 tePatchDistanceCtrl;

uniform mat4 MVPmatrix;

const float EPSILON = 0.00001;

bool floatEqual(in float a, in float b) {
	return abs(a - b) < EPSILON;
}

void main()
{
    float u = gl_TessCoord.x, v = gl_TessCoord.y;
	//quads go in lower right, upper right, upper left, lower left
    vec3 a = mix(gl_in[0].gl_Position.xyz, gl_in[3].gl_Position.xyz, u);
    vec3 b = mix(gl_in[1].gl_Position.xyz, gl_in[2].gl_Position.xyz, u);
    vec3 position = mix(a, b, v);

	if (!(floatEqual(v, 1) || floatEqual(v, 0))) {
		float radius = mix(tcNodeRadius[0], tcNodeRadius[1], v);
		vec3 dir = normalize(tcNodePosition[1] - tcNodePosition[0]);
		float projLength = dot(dir, position - tcNodePosition[0]);
		vec3 projection = tcNodePosition[0] + (dir * projLength);
		vec3 normal = normalize(position - projection);
		position = projection + (normal * radius);
	}

    tePatchDistance = vec4(u, v, 1-u, 1-v);
	
	float d01 = length(gl_in[0].gl_Position - gl_in[1].gl_Position);
	float d12 = length(gl_in[1].gl_Position - gl_in[2].gl_Position);
	float d23 = length(gl_in[2].gl_Position - gl_in[3].gl_Position);
	float d30 = length(gl_in[3].gl_Position - gl_in[0].gl_Position);
	tePatchDistanceCtrl = vec4(d01, d12, d23, d30);

	gl_Position = MVPmatrix * vec4(position, 1);
}
