#version 430

layout (location = 0) in vec2 Position;

layout(location = 3) uniform float texelSize; 
layout(location = 4) uniform bool horizontal; 

layout(location = 0) out vec2 left2Coordinate;
layout(location = 1) out vec2 left1Coordinate;
layout(location = 2) out vec2 centerCoordinate;
layout(location = 3) out vec2 right1Coordinate;
layout(location = 4) out vec2 right2Coordinate;

void main(void)
{
	vec2 filterStep = vec2(0.0);
	if (horizontal)
		filterStep.x = texelSize;
	else
		filterStep.y = texelSize;
	
	left2Coordinate = Position - 2*filterStep;
	left1Coordinate = Position - filterStep;
	centerCoordinate = Position;
	right1Coordinate = Position + filterStep;
	right2Coordinate = Position + 2*filterStep;
	
   vec2 pos = Position * 2 - vec2(1.0);
   gl_Position = vec4(pos, 0.0, 1.0);
}

