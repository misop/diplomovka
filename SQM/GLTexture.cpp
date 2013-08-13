#include "stdafx.h"
#include "GLTexture.h"


GLTexture::GLTexture(GLenum texType)
{
	glGenTextures(1, &texture);
	target = texType;
}

GLTexture::~GLTexture(void)
{
	glDeleteTextures(1, &texture);
}

void GLTexture::Bind() {
	glBindTexture(target, texture);
}

void GLTexture::FunctionTexture(int width, int height, float *data) {
	glTexImage2D(target, 0, GL_R32F, width, height, 0, GL_RED, GL_FLOAT, data);
}

void GLTexture::UseTexture(GLint loc) {
	glUniform1i(loc, texture);
}