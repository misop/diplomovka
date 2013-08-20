#include "stdafx.h"
#include "GLTexture.h"


GLTexture::GLTexture(GLenum texType)
{
	glGenTextures(1, &texture);
	glBindTexture(texType, texture);
	target = texType;

	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_NEAREST);    
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_NEAREST);
}

GLTexture::~GLTexture(void)
{
	glDeleteTextures(1, &texture);
}

void GLTexture::Enable() {
	glEnable(target);
}

void GLTexture::Disable() {
	glDisable(target);
}

void GLTexture::Bind() {
	glBindTexture(target, texture);
}

void GLTexture::FunctionTexture(int width, int height, float *data) {
	glTexImage2D(target, 0, GL_R32F, width, height, 0, GL_RED, GL_FLOAT, data);
}

void GLTexture::UseTexture(GLint loc, int unit) {
	glUniform1i(loc, unit);
}