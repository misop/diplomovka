#version 420

layout(quads) in;

in vec3 tcVertexNormal[];
in vec3 tcNodePosition[];
flat in int tcNodeType[];
flat in int tcNodeID[];

out vec4 tePatchDistance;
out vec4 tePatchDistanceCtrl;
out vec3 teColor;

uniform mat4 MVPmatrix;
layout(binding=0) uniform sampler2D RadiusesSampler;
layout(binding=1) uniform sampler2D CTS;
uniform float Threshold;
uniform int MaxID;

const float EPSILON = 0.00001;
const float threshold = 0.85;

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
	
	vec3 v1 = gl_in[1].gl_Position.xyz - gl_in[0].gl_Position.xyz;
	vec3 v2 = gl_in[3].gl_Position.xyz - gl_in[0].gl_Position.xyz;
	vec3 face_normal = normalize(cross(v1, v2));
	
	teColor = vec3(0, 0, u);
	int type0 = tcNodeType[0];
	int type1 = tcNodeType[1];

	//if (!((isBranchNode(type0) && floatEqual(v, 0)) || (isBranchNode(type1) && floatEqual(v, 1)))) {
	if (!(floatEqual(v, 1) || floatEqual(v, 0))) {
		//vec3 w0 = getNodePosition(tcNodeID[0]);		
		float x = texture(CTS, vec2(0, tcNodeID[0])).r;
		float y = texture(CTS, vec2(1, tcNodeID[0])).r;
		float z = texture(CTS, vec2(2, tcNodeID[0])).r;
		//vec3 w0 = vec3(x, y, z);
		vec3 w0 = tcNodePosition[0];
		
		x = texture(CTS, vec2(0, tcNodeID[1])).r;
		y = texture(CTS, vec2(1, tcNodeID[1])).r;
		z = texture(CTS, vec2(2, tcNodeID[1])).r;
		//vec3 w1 = vec3(x, y, z);
		vec3 w1 = tcNodePosition[1];
		vec3 d = normalize(w1 - w0);
		//float radius = mix(tcNodeRadius[0], tcNodeRadius[1], v);
		float radius = 0.0;
		float id0 = float(tcNodeID[0]) / float(MaxID);
		float id1 = float(tcNodeID[1]) / float(MaxID);
		float radius1 = texture(RadiusesSampler, vec2(id0, id1)).r;
		float radius2 = texture(RadiusesSampler, vec2(id1, id0)).r;
		//float radius1 = texture(RadiusesSampler, vec2(tcNodeID[0], tcNodeID[1])).r;
		//float radius2 = texture(RadiusesSampler, vec2(tcNodeID[1], tcNodeID[0])).r;

		float r0 = mix(radius2, radius1, v);
		float r1 = r0;
		if ((isConnectionNode(type0) || isLeafNode(type0)) && (isConnectionNode(type1) || isLeafNode(type1))) {
			//radius = mix(radius1, radius2, v);
			//already set up
		}
		if (isBranchNode(type0) && (isConnectionNode(type1) || isLeafNode(type1))) {
			teColor = vec3(abs(dot(face_normal, tcVertexNormal[0])), abs(dot(face_normal, tcVertexNormal[3])), 0);
			if (abs(dot(d, tcVertexNormal[0])) >= Threshold) {
				r0 = easein(radius1 / 2.0, radius2, v);
			}
			if (abs(dot(d, tcVertexNormal[3])) >= Threshold) {
				r1 = easein(radius1 / 2.0, radius2, v);
			}
		}
		if (isBranchNode(type1) && (isConnectionNode(type0) || isLeafNode(type0))) {
			//radius = easeout(radius1, radius2 / 2.0, v);
			teColor = vec3(abs(dot(face_normal, tcVertexNormal[1])), abs(dot(face_normal, tcVertexNormal[2])), 0);
			if (abs(dot(d, tcVertexNormal[1])) >= Threshold) {
				r0 = easeout(radius1, radius2 / 2.0, v);
			}
			if (abs(dot(d, tcVertexNormal[2])) >= Threshold) {
				r1 = easeout(radius1, radius2 / 2.0, v);
			}
		}
		if (isBranchNode(type0) && isBranchNode(type1)) {
			float t = Threshold - 0.5;
			bool b0 = abs(dot(d, tcVertexNormal[0])) >= t;
			bool b1 = abs(dot(d, tcVertexNormal[1])) >= t;
			bool b2 = abs(dot(d, tcVertexNormal[2])) >= t;
			bool b3 = abs(dot(d, tcVertexNormal[3])) >= t;

			if (b0 && b1) {
				r0 = easeinout(radius1 / 2.0, radius2 / 2.0, v);
			} else if (b0) {
				r0 = easein(radius1 / 2.0, radius2, v);
			} else if (b1) {
				r0 = easeout(radius1, radius2 / 2.0, v);
			}
			
			if (b3 && b2) {
				r1 = easeinout(radius1 / 2.0, radius2 / 2.0, v);
			} else if (b3) {
				r1 = easein(radius1 / 2.0, radius2, v);
			} else if (b2) {
				r1 = easeout(radius1, radius2 / 2.0, v);
			}
			//radius = easeinout(radius1 / 2.0, radius2 / 2.0, v);
		}
		
		radius = mix(r0, r1, u);
		if (r0 < r1)
			radius = easein(r0, r1, u);
		else
			radius = easeout(r0, r1, u);

		//radius = mix(mix(radius2, radius1, v), mix(radius2, radius1, v), u);

		//teColor = vec3(r0/max(r0,r1), r1/max(r0,r1), u);
		//teColor = vec3(r0/max(r0,r1), r1/max(r0,r1), 0);
		
		/*vec3 dir = normalize(tcNodePosition[1] - tcNodePosition[0]);
		float projLength = dot(dir, position - tcNodePosition[0]);
		vec3 projection = tcNodePosition[0] + (dir * projLength);*/
		vec3 dir = normalize(w1 - w0);
		float projLength = dot(dir, position - w0);
		vec3 projection = w0 + (dir * projLength);
		vec3 normal = normalize(position - projection);
		position = projection + (normal * radius);
		//position = mix(a, b, v);
	}
	
	//teColor = getNodePosition(tcNodeID[1]);	
	//teColor = texture(CTS, vec2(2, 1)).rgb;
	//teColor = vec3(id0, id1, 0);
    tePatchDistance = vec4(u, v, 1-u, 1-v);
	
	float d01 = length(gl_in[0].gl_Position - gl_in[1].gl_Position);
	float d12 = length(gl_in[1].gl_Position - gl_in[2].gl_Position);
	float d23 = length(gl_in[2].gl_Position - gl_in[3].gl_Position);
	float d30 = length(gl_in[3].gl_Position - gl_in[0].gl_Position);
	tePatchDistanceCtrl = vec4(d01, d12, d23, d30);

	gl_Position = MVPmatrix * vec4(position, 1);
}
