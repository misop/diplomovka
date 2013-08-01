#version 410

layout(triangles) in;
layout(triangle_strip, max_vertices = 3) out;

uniform mat3 NormalMatrix;

in int teTriPatch[];
in vec3 tePosition[3];
in vec4 tePatchDistance[3];
in vec4 tePatchDistanceCtrl[3];

out vec4 gPatchDistance;
out vec4 gPatchDistanceCtrl;
out vec3 gTriDistance;
out vec3 gTriDistanceCtrl;
out vec3 gVertex;
out vec3 gNormal;

void main()
{    
	vec3 p0 = vec3(gl_in[0].gl_Position);
	vec3 p1 = vec3(gl_in[1].gl_Position);
	vec3 p2 = vec3(gl_in[2].gl_Position);
	
	float d01 = length(p1 - p0);
	float d02 = length(p2 - p0);
	float d12 = length(p2 - p1);
	gTriDistanceCtrl = vec3(d01, d02, d12);

	gNormal = normalize(NormalMatrix * normalize(cross(p1 - p0, p2 - p0)));
	gVertex = (p0 + p1 + p2) / 3.0;

	gPatchDistanceCtrl = tePatchDistanceCtrl[0];

    gPatchDistance = tePatchDistance[0];
    gTriDistance = vec3(1, 0, 0);
    gl_Position = gl_in[0].gl_Position; EmitVertex();

    gPatchDistance = tePatchDistance[1];
    gTriDistance = vec3(0, 1, 0);
    gl_Position = gl_in[1].gl_Position; EmitVertex();

    gPatchDistance = tePatchDistance[2];
    gTriDistance = vec3(0, 0, 1);
    gl_Position = gl_in[2].gl_Position; EmitVertex();

    EndPrimitive();
}