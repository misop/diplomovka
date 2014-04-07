#version 430

layout (location = 0) in vec2 Position;

//layout(location = 0) uniform mat4 ModelMatrix;
//layout(location = 2) uniform mat4 ProjectionMatrix;

layout(location = 0) out vec2 uv;

void main(void)
{
   uv = Position;
   //gl_Position = ProjectionMatrix * ModelMatrix * vec4(Position, 0.0, 1.0);
   vec2 pos = Position * 2 - vec2(1.0);
   gl_Position = vec4(pos, 0.0, 1.0);
}

