#pragma once
#include <vector>
#include <string>
#include <glm\glm.hpp>
#include "GLArrayBuffer.h"
#include "GLTexture.h"

using namespace std;

class AssimpObject
{
	void LoadTexture(GLTexture **texture, string img);
public:
	GLArrayBuffer *buffer;
	GLTexture *diffuseTexture;
	GLTexture *displacementTexture;
	GLTexture *normalTexture;

	AssimpObject(void);
	~AssimpObject(void);
	
	void LoadFromFile(string fileName);
	void LoadDiffuseTexture(string img);
	void LoadDisplacementTexture(string img);
	void LoadNormalTexture(string img);
	void LoadTexturesFromFile(string fileName);
};

