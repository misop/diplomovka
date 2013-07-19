#version 410
 
in  vec3 teColor;

layout (location = 0) out vec4 fColor;
 
void main(void)
{
	fColor = vec4(teColor, 1.0);
}