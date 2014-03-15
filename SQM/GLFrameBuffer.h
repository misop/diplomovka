#pragma once
#include "GLTexture.h"
#include <GL\glew.h>
#include <GL\wglew.h>
#include <string>
#include <vector>

class GLFrameBuffer
{
	std::vector<GLenum> attachedTextures;
public:
	GLuint fbo;

	GLFrameBuffer(void);
	~GLFrameBuffer(void);
	
	void Bind();
	void AttachTexture(GLenum attachement, GLTexture *texture, bool bind = false);
	bool CheckStatus();
	void Draw();
};

