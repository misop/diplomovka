#version 430

in vec2 coords;

uniform sampler2D font;

const vec4 FontColor = vec4(1.0, 0.0, 0.0, 1.0);

layout (location = 0) out vec4 fColor;

void main(void)
{
   fColor = FontColor * texture(font, coords);
}
