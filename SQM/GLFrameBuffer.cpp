#include "stdafx.h"
#include "GLFrameBuffer.h"

using namespace std;

GLFrameBuffer::GLFrameBuffer(void) {
	glGenFramebuffers(1, &fbo);
}

GLFrameBuffer::~GLFrameBuffer(void) {
	glDeleteFramebuffers(1, &fbo);
}

void GLFrameBuffer::Bind() {
	glBindBuffer(GL_FRAMEBUFFER, fbo);
}

void GLFrameBuffer::AttachTexture(GLenum attachement, GLTexture *texture, bool bind) {
	if (bind) Bind();

	glFramebufferTexture2D(GL_FRAMEBUFFER, attachement, texture->target, texture->texture, 0);
	attachedTextures.push_back(attachement);
}

bool GLFrameBuffer::CheckStatus() {
	return glCheckFramebufferStatus(GL_FRAMEBUFFER) == GL_FRAMEBUFFER_COMPLETE;
}

void GLFrameBuffer::Draw() {
	Bind();

	glDrawBuffers(attachedTextures.size(), &attachedTextures[0]);

	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}