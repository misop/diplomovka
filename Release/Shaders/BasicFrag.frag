#version 410

layout (location = 0) out vec4 fColor;

uniform sampler2D RadiusesSampler;
 
void main(void)
{
	fColor = texture(RadiusesSampler, vec2(0,0));
	fColor.b = 1;
}