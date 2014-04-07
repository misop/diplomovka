#version 430

layout (location = 0) in vec2 Position;

layout(location = 0) uniform mat4 ModelMatrix;
layout(location = 2) uniform mat4 ProjectionMatrix;
layout(location = 3) uniform float texelWidth; 
layout(location = 4) uniform float texelHeight; 

out vec2 textureCoordinate;
out vec2 leftTextureCoordinate;
out vec2 rightTextureCoordinate;

out vec2 topTextureCoordinate;
out vec2 topLeftTextureCoordinate;
out vec2 topRightTextureCoordinate;

out vec2 bottomTextureCoordinate;
out vec2 bottomLeftTextureCoordinate;
out vec2 bottomRightTextureCoordinate;

void main(void)
{
	vec2 widthStep = vec2(texelWidth, 0.0);
	vec2 heightStep = vec2(0.0, texelHeight);
	vec2 widthHeightStep = vec2(texelWidth, texelHeight);
	vec2 widthNegativeHeightStep = vec2(texelWidth, -texelHeight);
	
	textureCoordinate = Position;
	leftTextureCoordinate = Position - widthStep;
	rightTextureCoordinate = Position + widthStep;

	topTextureCoordinate = Position - heightStep;
	topLeftTextureCoordinate = Position - widthHeightStep;
	topRightTextureCoordinate = Position + widthNegativeHeightStep;

	bottomTextureCoordinate = Position + heightStep;
	bottomLeftTextureCoordinate = Position - widthNegativeHeightStep;
	bottomRightTextureCoordinate = Position + widthHeightStep;
	
	gl_Position = ProjectionMatrix * ModelMatrix * vec4(Position, 0.0, 1.0);
}

