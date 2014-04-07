#version 430

layout (location = 0) in vec2 Position;

out vec2 uv;

void main(void)
{
   uv = Position;
   vec2 pos = Position * 2 - vec2(1.0);
   gl_Position = vec4(pos, 0.0, 1.0);
}

