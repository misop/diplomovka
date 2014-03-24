#version 430

layout (location = 0) in vec2 Position;

layout(location = 0) uniform mat4 ModelMatrix;
layout(location = 1) uniform bool swap;
layout(location = 2) uniform mat4 ProjectionMatrix;

out vec2 coords;

void main(void)
{
   coords = Position;
   if (swap) coords.y = 1 - coords.y;
   gl_Position = ProjectionMatrix * ModelMatrix * vec4(Position, 0.0, 1.0);
}

