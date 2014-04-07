#version 430

layout (location = 0) in vec2 Position;

layout(location = 3) uniform float texelSize; 
layout(location = 4) uniform bool horizontal; 

out vec2 left4Coordinate;
out vec2 left3Coordinate;
out vec2 left2Coordinate;
out vec2 left1Coordinate;
out vec2 centerCoordinate;
out vec2 right1Coordinate;
out vec2 right2Coordinate;
out vec2 right3Coordinate;
out vec2 right4Coordinate;

void main(void)
{
	vec2 filterStep = vec2(0.0);
	if (horizontal)
		filterStep.x = texelSize;
	else
		filterStep.y = texelSize;
	
	left4Coordinate = Position - 4*filterStep;
	left3Coordinate = Position - 3*filterStep;
	left2Coordinate = Position - 2*filterStep;
	left1Coordinate = Position - filterStep;
	centerCoordinate = Position;
	right1Coordinate = Position + filterStep;
	right2Coordinate = Position + 2*filterStep;
	right3Coordinate = Position + 3*filterStep;
	right4Coordinate = Position + 4*filterStep;
	
   vec2 pos = Position * 2 - vec2(1.0);
   gl_Position = vec4(pos, 0.0, 1.0);
}

