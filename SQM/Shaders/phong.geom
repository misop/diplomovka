#version 430

layout(triangles) in;
layout(triangle_strip, max_vertices = 3) out;

in vec4 v_vertex_eye[];
in vec4 v_normal_eye[];
in vec4 v_light_eye[];

out vec4 vertex_eye;
out vec4 normal_eye;
out vec4 light_eye;
out vec3 height;

void main(void) {	
	vertex_eye = v_vertex_eye[0];
    normal_eye = v_normal_eye[0];
	light_eye = v_light_eye[0];
    height = vec3(1, 0, 0);
    gl_Position = gl_in[0].gl_Position; EmitVertex();
	
	vertex_eye = v_vertex_eye[1];
    normal_eye = v_normal_eye[1];
	light_eye = v_light_eye[1];
    height = vec3(0, 1, 0);
    gl_Position = gl_in[1].gl_Position; EmitVertex();
	
	vertex_eye = v_vertex_eye[2];
    normal_eye = v_normal_eye[2];
	light_eye = v_light_eye[2];
    height = vec3(0, 0, 1);
    gl_Position = gl_in[2].gl_Position; EmitVertex();
	
    EndPrimitive();
}
