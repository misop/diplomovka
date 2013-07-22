#version 410

layout (triangles) in;
layout(triangle_strip, max_vertices = 3) out;

out vec3 gTriDistance;
out vec3 gTriDistanceCtrl;

void main(void)
{
	vec3 p0 = vec3(gl_in[0].gl_Position);
	vec3 p1 = vec3(gl_in[1].gl_Position);
	vec3 p2 = vec3(gl_in[2].gl_Position);
	
	float d01 = length(p1 - p0);
	float d02 = length(p2 - p0);
	float d12 = length(p2 - p1);

    gTriDistance = vec3(1, 0, 0);
	gTriDistanceCtrl = vec3(d01, d02, d12);
    gl_Position = gl_in[0].gl_Position;
    EmitVertex();

    gTriDistance = vec3(0, 1, 0);
	gTriDistanceCtrl = vec3(d01, d02, d12);
    gl_Position = gl_in[1].gl_Position;
    EmitVertex();

    gTriDistance = vec3(0, 0, 1);
	gTriDistanceCtrl = vec3(d01, d02, d12);
    gl_Position = gl_in[2].gl_Position;
    EmitVertex();

    EndPrimitive();
}
