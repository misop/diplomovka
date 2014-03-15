#pragma once
#include <GL\glew.h>
#include <GL\wglew.h>
#include <string>
#include <map>
#include <vector>

class GLTexture
{
public:
	GLuint texture;
	GLenum target;

	GLTexture(GLenum texType);
	~GLTexture(void);

	void Enable();
	void Disable();
	void Bind();
	void FunctionTexture(int width, int height, float *data);
	void UseTexture(GLint loc, int unit);
};

