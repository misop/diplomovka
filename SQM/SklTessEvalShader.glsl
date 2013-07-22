#version 410

layout(triangles, equal_spacing, cw) in;

uniform mat4 MVPmatrix;
uniform mat4 ModelMatrix;

void main()
{
    vec3 p0 = gl_TessCoord.x * gl_in[0].gl_Position.xyz;
    vec3 p1 = gl_TessCoord.y * gl_in[1].gl_Position.xyz;
    vec3 p2 = gl_TessCoord.z * gl_in[2].gl_Position.xyz;

    //tePosition = normalize(p0 + p1 + p2);
	vec3 position = normalize(p0 + p1 + p2);

	gl_Position = MVPmatrix * (ModelMatrix * vec4(position, 1));
    //gl_Position = MVPmatrix * ModelMatrix * vec4(tePosition, 1);
    //gl_Position = vec4(tePosition, 1);
}
