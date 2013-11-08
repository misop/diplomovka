#version 420

layout(triangles, equal_spacing, cw) in;

out vec4 tePatchDistance;
out vec4 tePatchDistanceCtrl;
out vec3 tePos;

uniform mat4 MVPmatrix;

void main()
{
	float maxi = max(max(gl_TessCoord.x, gl_TessCoord.y), gl_TessCoord.z);
    tePatchDistance = vec4(gl_TessCoord, maxi);

    vec3 p0 = gl_TessCoord.x * gl_in[0].gl_Position.xyz;
    vec3 p1 = gl_TessCoord.y * gl_in[1].gl_Position.xyz;
    vec3 p2 = gl_TessCoord.z * gl_in[2].gl_Position.xyz;
	vec3 position = (p0 + p1 + p2);

	float d01 = length(gl_in[0].gl_Position - gl_in[1].gl_Position);
	float d12 = length(gl_in[1].gl_Position - gl_in[2].gl_Position);
	float d23 = length(gl_in[2].gl_Position - gl_in[3].gl_Position);
	float d30 = max(max(d01, d12), d23);
	tePatchDistanceCtrl = vec4(d01, d12, d23, d30);
	
	//gl_Position = MVPmatrix * vec4(gl_TessCoord, 1);
	gl_Position = MVPmatrix * vec4(position, 1);
	tePos = position;
}
