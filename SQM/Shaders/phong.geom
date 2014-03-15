#version 430

layout(triangles) in;
layout(triangle_strip, max_vertices = 3) out;

in _{
	vec4 vertex_eye;
	vec4 normal_eye;
	vec4 light_eye;
	vec2 uv;
} te[];

out _{
	vec4 vertex_eye;
	vec4 normal_eye;
	vec4 light_eye;
	vec3 height;
	vec2 uv;
} g;

void main(void) {	
	g.vertex_eye = te[0].vertex_eye;
    g.normal_eye = te[0].normal_eye;
	g.light_eye = te[0].light_eye;
	g.uv = te[0].uv;
    g.height = vec3(1, 0, 0);
    gl_Position = gl_in[0].gl_Position; EmitVertex();
	
	g.vertex_eye = te[1].vertex_eye;
    g.normal_eye = te[1].normal_eye;
	g.light_eye = te[1].light_eye;
	g.uv = te[1].uv;
    g.height = vec3(0, 1, 0);
    gl_Position = gl_in[1].gl_Position; EmitVertex();
	
	g.vertex_eye = te[2].vertex_eye;
    g.normal_eye = te[2].normal_eye;
	g.light_eye = te[2].light_eye;
	g.uv = te[2].uv;
    g.height = vec3(0, 0, 1);
    gl_Position = gl_in[2].gl_Position; EmitVertex();
	
    EndPrimitive();
}
