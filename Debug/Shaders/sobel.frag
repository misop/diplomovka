#version 430

in vec2 textureCoordinate;
in vec2 leftTextureCoordinate;
in vec2 rightTextureCoordinate;

in vec2 topTextureCoordinate;
in vec2 topLeftTextureCoordinate;
in vec2 topRightTextureCoordinate;

in vec2 bottomTextureCoordinate;
in vec2 bottomLeftTextureCoordinate;
in vec2 bottomRightTextureCoordinate;

layout(binding=0) uniform sampler2D ImageSampler;

const mat3 SobelX = mat3(-1, 0, 1,
						 -2, 0, 2,
						 -1, 0, 1);
						 
const mat3 SobelY = mat3( 1,  2,  1,
						  0,  0,  0,
						 -1, -2, -1);
				
layout (location = 0) out vec4 fColor;

void main(void)
{
	vec4 bottomColor = texture2D(ImageSampler, bottomTextureCoordinate);
	vec4 bottomLeftColor = texture2D(ImageSampler, bottomLeftTextureCoordinate);
	vec4 bottomRightColor = texture2D(ImageSampler, bottomRightTextureCoordinate);
	vec4 centerColor = texture2D(ImageSampler, textureCoordinate);
	vec4 leftColor = texture2D(ImageSampler, leftTextureCoordinate);
	vec4 rightColor = texture2D(ImageSampler, rightTextureCoordinate);
	vec4 topColor = texture2D(ImageSampler, topTextureCoordinate);
	vec4 topRightColor = texture2D(ImageSampler, topRightTextureCoordinate);
	vec4 topLeftColor = texture2D(ImageSampler, topLeftTextureCoordinate);

	vec4 edgeX = topLeftColor * SobelX[0][0] + topColor * SobelX[0][1] + topRightColor * SobelX[0][2];
	edgeX += leftColor * SobelX[1][0] + centerColor * SobelX[1][1] + rightColor * SobelX[1][2];
	edgeX += bottomLeftColor * SobelX[2][0] + bottomColor * SobelX[2][1] + bottomRightColor * SobelX[2][2];
	
	vec4 edgeY = topLeftColor * SobelY[0][0] + topColor * SobelY[0][1] + topRightColor * SobelY[0][2];
	edgeY += leftColor * SobelY[1][0] + centerColor * SobelY[1][1] + rightColor * SobelY[1][2];
	edgeY += bottomLeftColor * SobelY[2][0] + bottomColor * SobelY[2][1] + bottomRightColor * SobelY[2][2];
	
	vec4 edge = sqrt(edgeX*edgeX + edgeY*edgeY);

	float detected = edge.r + edge.g + edge.b + edge.a;
	if (detected < 2) discard;//detected = 0;
	//fColor = vec4(detected);
	fColor = vec4(0);
	fColor.a = 1 - centerColor.a;// *0.8;
}
