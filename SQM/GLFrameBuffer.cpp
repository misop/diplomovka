#include "stdafx.h"
#include "GLFrameBuffer.h"

using namespace std;

GLFrameBuffer::GLFrameBuffer(void) : renderBuffer(0) {
	glGenFramebuffers(1, &fbo);
}

GLFrameBuffer::~GLFrameBuffer(void) {
	glDeleteRenderbuffers(1, &renderBuffer);
	for (int i = 0; i < attachedTextures.size(); i++) {
		delete attachedTextures[i];
	}
	glDeleteFramebuffers(1, &fbo);
}

void GLFrameBuffer::Bind() {
	glBindFramebuffer(GL_FRAMEBUFFER, fbo);
}

void GLFrameBuffer::Unbind() {
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

bool GLFrameBuffer::CheckStatus() {
	return glCheckFramebufferStatus(GL_FRAMEBUFFER) == GL_FRAMEBUFFER_COMPLETE;
}

bool GLFrameBuffer::CreateGeneralFBO(int width, int height, int colorBuffers, bool depthBuffer) {
	// zisti ci sme neprekrocili povoleny pocet color bufrov
	if (colorBuffers > 16)
		colorBuffers = 16;
	GLint maxColorAttachments;
	glGetIntegerv(GL_MAX_COLOR_ATTACHMENTS, &maxColorAttachments);
	if (colorBuffers > maxColorAttachments)
		colorBuffers = maxColorAttachments;
	// vytvor FBO
	glBindFramebuffer(GL_FRAMEBUFFER_EXT, fbo);
	// vytvor dany pocet texture a pripoj ich ku color bufrom
	for (int i = 0; i < colorBuffers; i++) {
		GLTexture *texture = new GLTexture(GL_TEXTURE_2D);
		texture->Bind();
		texture->TexParameteri(GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		texture->TexParameteri(GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		texture->TexParameteri(GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		texture->TexParameteri(GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		texture->RGBATexture(width, height, GL_RGBA, GL_FLOAT, 0);

		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + i, GL_TEXTURE_2D, texture->texture, 0);
		attachedTextures.push_back(texture);
	}

	// pridaj texturu alebo renderbuffer ako depth buffer
	if (depthBuffer) {
		GLTexture *depth = new GLTexture(GL_TEXTURE_2D);
		depth->Bind();
		depth->TexParameteri(GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		depth->TexParameteri(GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		//depth->TexParameteri(GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		//depth->TexParameteri(GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		//depth->TexParameteri(GL_TEXTURE_COMPARE_MODE, GL_NONE);
		depth->TexParameteri(GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		depth->TexParameteri(GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		depth->TexParameteri(GL_TEXTURE_COMPARE_MODE, GL_COMPARE_REF_TO_TEXTURE);
		depth->TexParameteri(GL_TEXTURE_COMPARE_FUNC, GL_LEQUAL);
		depth->DepthTexture(width, height, GL_FLOAT);

		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, depth->texture, 0);
		attachedTextures.push_back(depth);
	} else {
		glGenRenderbuffers(1, &renderBuffer);
		glBindRenderbuffer(GL_RENDERBUFFER, renderBuffer);
		glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT24, width, height);
		glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, renderBuffer);
		glBindRenderbuffer(GL_RENDERBUFFER, 0);
	}

	// nastav buffre pre zapis
	if (colorBuffers == 0)
		glDrawBuffer(GL_NONE);
	else if (colorBuffers == 1)
		glDrawBuffer(GL_COLOR_ATTACHMENT0);
	else {
		GLuint attachments[16] = {
			GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1, GL_COLOR_ATTACHMENT2, GL_COLOR_ATTACHMENT3,
			GL_COLOR_ATTACHMENT4, GL_COLOR_ATTACHMENT5, GL_COLOR_ATTACHMENT6, GL_COLOR_ATTACHMENT7,
			GL_COLOR_ATTACHMENT8, GL_COLOR_ATTACHMENT9, GL_COLOR_ATTACHMENT10, GL_COLOR_ATTACHMENT11,
			GL_COLOR_ATTACHMENT12, GL_COLOR_ATTACHMENT13, GL_COLOR_ATTACHMENT14, GL_COLOR_ATTACHMENT15
		};
		glDrawBuffers(colorBuffers, attachments);
	}

	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	return CheckStatus();
}