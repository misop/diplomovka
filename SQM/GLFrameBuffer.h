#pragma once
#include "GLTexture.h"
#include <GL\glew.h>
#include <GL\wglew.h>
#include <string>
#include <vector>

class GLFrameBuffer
{
	GLuint renderBuffer;
public:
	std::vector<GLTexture*> attachedTextures;
	GLuint fbo;

	GLFrameBuffer(void);
	~GLFrameBuffer(void);
	
	bool CreateGeneralFBO(int width, int height, int colorBuffers, bool depthBuffer);
	void Bind();
	void Unbind();
	bool CheckStatus();
};

