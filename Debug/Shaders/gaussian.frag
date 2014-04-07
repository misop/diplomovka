 #version 430

in vec2 left4Coordinate;
in vec2 left3Coordinate;
in vec2 left2Coordinate;
in vec2 left1Coordinate;
in vec2 centerCoordinate;
in vec2 right1Coordinate;
in vec2 right2Coordinate;
in vec2 right3Coordinate;
in vec2 right4Coordinate;

layout(binding=0) uniform sampler2D ImageSampler;

//const float Gaussian[9] = float[](0, 0, 1, 4, 7, 4, 1, 0, 0);
const float Gaussian[9] = float[](1, 3, 5, 7, 8, 7, 5, 3, 1);
//const float Gaussian[9] = float[](0, 1, 2, 3, 4, 3, 2, 1, 0);
//const float Gaussian[5] = float[](1.0/17.0, 4.0/17.0, 7.0/17.0, 4.0/17.0, 1.0/17.0);
const float epsilon = 1.0;

#define CENTER 4
				
layout (location = 0) out vec4 fColor;

void main(void)
{
	float normalization = 1.0/epsilon * Gaussian[CENTER];
	float closeness;
	float sampleWeight;
	vec4 result = vec4(0.0);
	
	vec4 left4Color = texture2D(ImageSampler, left4Coordinate);
	vec4 left3Color = texture2D(ImageSampler, left3Coordinate);
	vec4 left2Color = texture2D(ImageSampler, left2Coordinate);
	vec4 left1Color = texture2D(ImageSampler, left1Coordinate);
	vec4 centerColor = texture2D(ImageSampler, centerCoordinate);
	vec4 right1Color = texture2D(ImageSampler, right1Coordinate);
	vec4 right2Color = texture2D(ImageSampler, right2Coordinate);
	vec4 right3Color = texture2D(ImageSampler, right3Coordinate);
	vec4 right4Color = texture2D(ImageSampler, right4Coordinate);
	
	if (centerColor.a < 0.01) discard;
	
	result = centerColor * 1.0/epsilon * Gaussian[CENTER];
	//fColor = Gaussian[0]*left2Color + Gaussian[1]*left1Color + Gaussian[2]*centerColor + Gaussian[3]*right1Color + Gaussian[4]*right2Color;
	//result = (left2Color + left1Color + centerColor + right1Color + right2Color) / 5.0;
	
	//left 4
	closeness = 1.0 / (epsilon + abs(centerColor.a - left3Color.a));
	sampleWeight = closeness * Gaussian[0];
	result += left4Color * sampleWeight;
	normalization += sampleWeight;
	//left 3
	closeness = 1.0 / (epsilon + abs(centerColor.a - left4Color.a));
	sampleWeight = closeness * Gaussian[1];
	result += left3Color * sampleWeight;
	normalization += sampleWeight;
	//left 2
	closeness = 1.0 / (epsilon + abs(centerColor.a - left2Color.a));
	sampleWeight = closeness * Gaussian[2];
	result += left2Color * sampleWeight;
	normalization += sampleWeight;
	//left 1
	closeness = 1.0 / (epsilon + abs(centerColor.a - left1Color.a));
	sampleWeight = closeness * Gaussian[3];
	result += left1Color * sampleWeight;
	normalization += sampleWeight;
	//center has 0 distance
	//right 1
	closeness = 1.0 / (epsilon + abs(centerColor.a - right1Color.a));
	sampleWeight = closeness * Gaussian[5];
	result += right1Color * sampleWeight;
	normalization += sampleWeight;
	//right 2
	closeness = 1.0 / (epsilon + abs(centerColor.a - right2Color.a));
	sampleWeight = closeness * Gaussian[6];
	result += right2Color * sampleWeight;
	normalization += sampleWeight;
	//right 3
	closeness = 1.0 / (epsilon + abs(centerColor.a - right3Color.a));
	sampleWeight = closeness * Gaussian[7];
	result += right3Color * sampleWeight;
	normalization += sampleWeight;
	//right 4
	closeness = 1.0 / (epsilon + abs(centerColor.a - right4Color.a));
	sampleWeight = closeness * Gaussian[8];
	result += right4Color * sampleWeight;
	normalization += sampleWeight;
	
	fColor = result / normalization;
}
