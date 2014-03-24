 #version 430

in vec2 left2Coordinate;
in vec2 left1Coordinate;
in vec2 centerCoordinate;
in vec2 right1Coordinate;
in vec2 right2Coordinate;

layout(binding=0) uniform sampler2D ImageSampler;

const float Gaussian[5] = float[](1, 4, 7, 4, 1);
const float epsilon = 1.0;
				
layout (location = 0) out vec4 fColor;

void main(void)
{
	float normalization = 1.0/epsilon * Gaussian[2];
	float closeness;
	float sampleWeight;
	vec4 result = vec4(0.0);
	
	vec4 left2Color = texture2D(ImageSampler, left2Coordinate);
	vec4 left1Color = texture2D(ImageSampler, left1Coordinate);
	vec4 centerColor = texture2D(ImageSampler, centerCoordinate);
	vec4 right1Color = texture2D(ImageSampler, right1Coordinate);
	vec4 right2Color = texture2D(ImageSampler, right2Coordinate);
	
	result = centerColor * 1.0/epsilon * Gaussian[2];
	//result = Gaussian[0]*left2Color + Gaussian[1]*left1Color + Gaussian[2]*centerColor + Gaussian[3]*right1Color + Gaussian[4]*right2Color;
	
	//left 2
	closeness = 1.0 / (epsilon + abs(centerColor.a - left2Color.a));
	sampleWeight = closeness * Gaussian[0];
	result += left2Color * sampleWeight;
	normalization += sampleWeight;
	//left 1
	closeness = 1.0 / (epsilon + abs(centerColor.a - left1Color.a));
	sampleWeight = closeness * Gaussian[1];
	result += left1Color * sampleWeight;
	normalization += sampleWeight;
	//center has 0 distance
	//right 1
	closeness = 1.0 / (epsilon + abs(centerColor.a - right1Color.a));
	sampleWeight = closeness * Gaussian[3];
	result += right1Color * sampleWeight;
	normalization += sampleWeight;
	//right 2
	closeness = 1.0 / (epsilon + abs(centerColor.a - right2Color.a));
	sampleWeight = closeness * Gaussian[4];
	result += right2Color * sampleWeight;
	normalization += sampleWeight;
	
	fColor = result / normalization;
}
