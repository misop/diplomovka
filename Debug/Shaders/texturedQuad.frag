#version 430

in vec2 uv;

layout(binding=0) uniform sampler2D Sampler;

layout (location = 0) out vec4 fColor;

void main(void)
{
	fColor = texture(Sampler, uv);
}
