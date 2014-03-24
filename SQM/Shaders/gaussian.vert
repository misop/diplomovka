#version 430

layout (location = 0) in vec2 Position;

layout(location = 0) uniform mat4 ModelMatrix;
layout(location = 2) uniform mat4 ProjectionMatrix;
layout(location = 3) uniform float texelSize; 
layout(location = 4) uniform bool horizontal; 

out vec2 left2Coordinate;
out vec2 left1Coordinate;
out vec2 centerCoordinate;
out vec2 right1Coordinate;
out vec2 right2Coordinate;

void main(void)
{
	vec2 filterStep = vec2(0.0);
	if (horizontal)
		filterStep.x = texelSize;
	else
		filterStep.y = texelSize;
	
	vec2 left2Coordinate = Position - 2*filterStep;
	vec2 left1Coordinate = Position - filterStep;
	vec2 centerCoordinate = Position;
	vec2 right1Coordinate = Position + filterStep;
	vec2 right2Coordinate = Position + 2*filterStep;
	
	gl_Position = ProjectionMatrix * ModelMatrix * vec4(Position, 0.0, 1.0);
}

