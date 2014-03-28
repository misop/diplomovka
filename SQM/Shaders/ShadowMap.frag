#version 430

in vec3 position;

const float zNear = 1.0;
const float zFar = 15000;
const float dist = 3700;

layout (location = 0) out vec4 fColor;

void main(void)
{
    float z_b = gl_FragCoord.z;
    float z_n = 2.0 * z_b - 1.0;
    float z_e = 2.0 * zNear * zFar / (zFar + zNear - z_n * (zFar - zNear));
	z_e /= (zFar - zNear);
	
	fColor = vec4(vec3(position.z), 1.0);
	//fColor = vec4(vec3(z_e), 1.0);
}
