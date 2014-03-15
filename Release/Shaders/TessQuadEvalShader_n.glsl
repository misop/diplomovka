#version 420

#define SKINNING_MATRICES fill_in_skinnig_matrices_number

layout(quads) in;

uniform mat4 SkinningMatrices[SKINNING_MATRICES];
uniform mat4 TransformMatrices[SKINNING_MATRICES];

uniform mat4 MVPmatrix;
out vec4 tePatchDistance;
out vec4 tePatchDistanceCtrl;


void main()
{
	mat4 B = mat4 (
        -1, 3, -3, 1,
        3, -6, 3, 0,
        -3, 3, 0, 0,
        1, 0, 0, 0 );
	mat4 BT = transpose(B);

    float u = gl_TessCoord.x, v = gl_TessCoord.y;
    mat4 Px = mat4(
        gl_in[0].gl_Position.x, gl_in[0].gl_Position.x, gl_in[0].gl_Position.x, gl_in[0].gl_Position.x,
        gl_in[1].gl_Position.x, gl_in[1].gl_Position.x, gl_in[1].gl_Position.x, gl_in[1].gl_Position.x,
        gl_in[2].gl_Position.x, gl_in[2].gl_Position.x, gl_in[2].gl_Position.x, gl_in[2].gl_Position.x,
        gl_in[3].gl_Position.x, gl_in[3].gl_Position.x, gl_in[3].gl_Position.x, gl_in[3].gl_Position.x );
    mat4 Py = mat4(
        gl_in[0].gl_Position.y, gl_in[0].gl_Position.y, gl_in[0].gl_Position.y, gl_in[0].gl_Position.y,
        gl_in[1].gl_Position.y, gl_in[1].gl_Position.y, gl_in[1].gl_Position.y, gl_in[1].gl_Position.y,
        gl_in[2].gl_Position.y, gl_in[2].gl_Position.y, gl_in[2].gl_Position.y, gl_in[2].gl_Position.y,
        gl_in[3].gl_Position.y, gl_in[3].gl_Position.y, gl_in[3].gl_Position.y, gl_in[3].gl_Position.y );
    mat4 Pz = mat4(
        gl_in[0].gl_Position.z, gl_in[0].gl_Position.z, gl_in[0].gl_Position.z, gl_in[0].gl_Position.z,
        gl_in[1].gl_Position.z, gl_in[1].gl_Position.z, gl_in[1].gl_Position.z, gl_in[1].gl_Position.z,
        gl_in[2].gl_Position.z, gl_in[2].gl_Position.z, gl_in[2].gl_Position.z, gl_in[2].gl_Position.z,
        gl_in[3].gl_Position.z, gl_in[3].gl_Position.z, gl_in[3].gl_Position.z, gl_in[3].gl_Position.z );
    mat4 cx = B * Px * BT;
    mat4 cy = B * Py * BT;
    mat4 cz = B * Pz * BT;
    vec4 U = vec4(u*u*u, u*u, u, 1);
    vec4 V = vec4(v*v*v, v*v, v, 1);
    float x = dot(cx * V, U);
    float y = dot(cy * V, U);
    float z = dot(cz * V, U);
    tePatchDistance = vec4(u, v, 1-u, 1-v);
	tePatchDistanceCtrl = vec4(0,0,0,0);
    gl_Position = MVPmatrix * vec4(x, y, z, 1);
}
