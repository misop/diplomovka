#version 430

#define POINT_LIGHTS 8

layout(triangles) in;
layout(triangle_strip, max_vertices = 3) out;

layout(location = 0) in vec4 i_vertex_eye[];
layout(location = 1) in vec4 i_normal_eye[];
layout(location = 2) in vec4 i_light_eye[];
layout(location = 5) in vec2 i_uv[];

layout(location = 0) out vec4 vertex_eye;
layout(location = 1) out vec4 normal_eye;
layout(location = 2) out vec4 light_eye;
layout(location = 3) out vec2 uv;
layout(location = 4) out vec3 height;

void main(void) {	
	vertex_eye = i_vertex_eye[0];
    normal_eye = i_normal_eye[0];
	light_eye = i_light_eye[0];
	uv = i_uv[0];
    height = vec3(1, 0, 0);
    gl_Position = gl_in[0].gl_Position; EmitVertex();
	
	vertex_eye = i_vertex_eye[1];
    normal_eye = i_normal_eye[1];
	light_eye = i_light_eye[1];
	uv = i_uv[1];
    height = vec3(0, 1, 0);
    gl_Position = gl_in[1].gl_Position; EmitVertex();
	
	vertex_eye = i_vertex_eye[2];
    normal_eye = i_normal_eye[2];
	light_eye = i_light_eye[2];
	uv = i_uv[2];
    height = vec3(0, 0, 1);
    gl_Position = gl_in[2].gl_Position; EmitVertex();
	
    EndPrimitive();
}
