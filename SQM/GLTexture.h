#pragma once
#include <GL\glew.h>
#include <GL\wglew.h>
#include <string>
#include <map>
#include <vector>

class GLTexture
{
	GLuint texture;
	GLenum target;
public:
	GLTexture(GLenum texType);
	~GLTexture(void);

	void Bind();
	void FunctionTexture(int width, int height, float *data);
	void UseTexture(GLint loc);
};

