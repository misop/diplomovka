#version 420

layout(triangles, equal_spacing, cw) in;

uniform mat4 MVPmatrix;
uniform mat4 ModelMatrix;
uniform mat4 ViewMatrix;
uniform mat3 NormalMatrix;

layout(location = 0) out vec4 vertex_eye;
layout(location = 1) out vec4 normal_eye;
layout(location = 2) out vec4 light_eye;

void main()
{
    vec3 p0 = gl_TessCoord.x * gl_in[0].gl_Position.xyz;
    vec3 p1 = gl_TessCoord.y * gl_in[1].gl_Position.xyz;
    vec3 p2 = gl_TessCoord.z * gl_in[2].gl_Position.xyz;

    //tePosition = normalize(p0 + p1 + p2);
	vec3 position = normalize(p0 + p1 + p2);

	
	vertex_eye = ViewMatrix * ModelMatrix * vec4(position, 1);
	light_eye = vec4(0, 0, 0, 1) - vertex_eye;
	//normal_eye = ViewMatrix * ModelMatrix * vec4(position, 0);
	normal_eye = vec4(NormalMatrix * position, 0);
	vertex_eye = vec4(0, 0, 0, 1) - vertex_eye;
	
	gl_Position = MVPmatrix * (ModelMatrix * vec4(position, 1));
    //gl_Position = MVPmatrix * ModelMatrix * vec4(tePosition, 1);
    //gl_Position = vec4(tePosition, 1);
}
