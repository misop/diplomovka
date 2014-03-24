#version 430

layout(location = 0) in vec3 normal_eye;

layout (location = 0) out vec4 fColor;
 
const float zNear = 1.0;
const float zFar = 15000;
const float dist = 3700;

void main(void)
{
    float z_b = gl_FragCoord.z;
    float z_n = 2.0 * z_b - 1.0;
    float z_e = 2.0 * zNear * zFar / (zFar + zNear - z_n * (zFar - zNear));
	z_e /= dist;
	
	//fColor = vec4(normalize(normal_eye), z_e);
	//should be view space but look better screen space
	fColor = vec4(normalize(normal_eye), z_e);
}