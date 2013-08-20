#version 410

layout(quads) in;

in vec3 tcNodePosition[];
in int tcNodeType[];
in int tcNodeID[];

out vec4 tePatchDistance;
out vec4 tePatchDistanceCtrl;

uniform mat4 MVPmatrix;
uniform sampler2D RadiusesSampler;

const float EPSILON = 0.00001;

bool floatEqual(in float a, in float b) {
	return abs(a - b) < EPSILON;
}

const vec2 P0 = vec2(0.0, 0.0);
const vec2 P3 = vec2(1.0, 1.0);

vec2 bezier(in vec2 P1, in vec2 P2, in float t) {
	return (pow(1 - t, 3)*P0 + 3*pow(1 - t, 2)*t*P1 + 3*(1 - t)*pow(t, 2)*P2 + pow(t, 3)*P3);
}

float easein(in float start, in float end, in float t) {
	vec2 P1 = vec2(0.55, 0.055);
	vec2 P2 = vec2(0.675, 0.19);
	vec2 P = bezier(P1, P2, t);
	float percent = P.y;
	return (start + (end - start)*percent);
}

float easeout(in float start, in float end, in float t) {
	vec2 P1 = vec2(0.215, 0.61);
	vec2 P2 = vec2(0.355, 1.0);
	vec2 P = bezier(P1, P2, t);
	float percent = P.y;
	return (start + (end - start)*percent);
}

float easeinout(in float start, in float end, in float t) {
	vec2 P1 = vec2(0.77, 0.0);
	vec2 P2 = vec2(0.175, 1.0);
	vec2 P = bezier(P1, P2, t);
	float percent = P.y;
	return (start + (end - start)*percent);
}

bool isConnectionNode(in int type) {
	return (type == 0);
}

bool isBranchNode(in int type) {
	return (type == 1);
}

bool isLeafNode(in int type) {
	return (type == 2);
}

void main()
{
    float u = gl_TessCoord.x, v = gl_TessCoord.y;
	//quads go in lower right, upper right, upper left, lower left
    vec3 a = mix(gl_in[0].gl_Position.xyz, gl_in[3].gl_Position.xyz, u);
    vec3 b = mix(gl_in[1].gl_Position.xyz, gl_in[2].gl_Position.xyz, u);
    vec3 position = mix(a, b, v);

	if (!(floatEqual(v, 1) || floatEqual(v, 0))) {
		//float radius = mix(tcNodeRadius[0], tcNodeRadius[1], v);
		float radius = 0.0;
		//float radius1 = texture(RadiusesSampler, vec2(tcNodeID[0], tcNodeID[1])).r;
		float radius1 = texture(RadiusesSampler, vec2(tcNodeID[0], tcNodeID[1])).r;
		float radius2 = texture(RadiusesSampler, vec2(tcNodeID[1], tcNodeID[0])).r;

		int type0 = tcNodeType[0];
		int type1 = tcNodeType[1];

		radius = mix(radius1, radius2, v);
		if ((isConnectionNode(type0) || isLeafNode(type0)) && (isConnectionNode(type1) || isLeafNode(type1))) {
			radius = mix(radius1, radius2, v);
		}
		if (isBranchNode(type0) && (isConnectionNode(type1) || isLeafNode(type1))) {
			radius = easein(radius1 / 2.0, radius2, v);
		}
		if (isBranchNode(type1) && (isConnectionNode(type0) || isLeafNode(type0))) {
			radius = easeout(radius1, radius2 / 2.0, v);
		}
		if (isBranchNode(type0) && isBranchNode(type1)) {
			radius = easeinout(radius1 / 2.0, radius2 / 2.0, v);
		}

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
