#version 430

layout (location = 0) in vec3 Position;

layout(location = 0) uniform mat4 ModelMatrix;
layout(location = 1) uniform mat4 ViewMatrix;
layout(location = 2) uniform mat4 ProjectionMatrix;

out vec3 cube_map_coords;

void main(void)
{
   cube_map_coords = Position.xyz;
   gl_Position = ProjectionMatrix * vec4(mat3(ViewMatrix) * vec3(ModelMatrix * vec4(Position, 1.0)), 1);
}

