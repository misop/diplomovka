#version 430

in vec2 coords;

layout(binding=0) uniform sampler2D font;
layout(location = 1) uniform bool swap;

const float zNear = 1.0;
const float zFar = 15000;//0xFFFFFFFF;

layout (location = 0) out vec4 fColor;

void main(void)
{
	vec4 text = texture(font, coords);	
	//float depth = texture(font, coords).r;	
    //float z_n = 2.0 * depth - 1.0;
    //float z_e = 2.0 * zNear * zFar / (zFar + zNear - z_n * (zFar - zNear));
	if (swap && text.a < 0.01) discard;
	fColor = text;
	//fColor = vec4(z_e);
}
