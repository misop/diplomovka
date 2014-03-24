#version 430

in vec3 cube_map_coords;

uniform samplerCube cube_map;
layout(location = 7) uniform bool ToonShading;

const float Toonines = 15;
const float Toonines_1 = 1.0/Toonines;

layout (location = 0) out vec4 fColor;

void main(void)
{
   fColor = texture(cube_map, cube_map_coords);;
   if (ToonShading) {
		fColor = floor(fColor * Toonines) * Toonines_1;
   }
   //fColor = vec4(0, 0, 0, 1);
}
