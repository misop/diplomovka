#version 430

in vec2 uv;

layout(binding=0) uniform sampler2D ImageSampler;

layout (location = 0) out vec4 fColor;

void main(void)
{
	vec4 color = texture(ImageSampler, uv);	
	if (color.a < 0.1) discard;
	fColor = vec4(0.0);
	fColor.a = 1 - color.r;
}
