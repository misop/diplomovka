#pragma once
#include <vector>
#include <string>
#include <memory>
#include <glm\glm.hpp>
#include "GLArrayBuffer.h"
#include "GLTexture.h"

using namespace std;

class AssimpObject
{
public:
	GLArrayBuffer *buffer;
	shared_ptr<GLTexture> diffuseTexture;
	shared_ptr<GLTexture> displacementTexture;
	shared_ptr<GLTexture> normalTexture;

	AssimpObject(void);
	~AssimpObject(void);
	
	void LoadFromFile(string fileName);
	void LoadTexturesFromFile(string fileName);
};

