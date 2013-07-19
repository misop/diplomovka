#include "stdafx.h"
#include "GLArrayBuffer.h"


GLArrayBuffer::GLArrayBuffer(void)
{
	glGenVertexArrays(1, &vao);
	attribs = 0;
	vertices = 0;
}


GLArrayBuffer::~GLArrayBuffer(void)
{
	if (vbos.size() > 0) {
		glDeleteBuffers(vbos.size(), &vbos[0]);
		vbos.clear();
	}
	glDeleteVertexArrays(1, &vao);
}

void GLArrayBuffer::Bind() {
	glBindVertexArray(vao);
}

void GLArrayBuffer::Draw(GLenum mode, bool bind) {
	if (vertices == 0) return;
	if (bind) glBindVertexArray(vao);

	glDrawArrays(mode, 0, vertices); 
}

void GLArrayBuffer::BindBufferData(std::vector<float> data, GLint elements, GLenum storageMode, GLboolean normalize, GLsizei stride, GLvoid* offset, bool bind) {
	if (data.size() == 0) return;
	if (bind) glBindVertexArray(vao);
	if (attribs == 0) {//0 index is reserved for vertices
		vertices = (int)floor((float)data.size()/(float)elements);
	}

	glEnableVertexAttribArray(attribs);

	GLuint vbo;
	glGenBuffers(1, &vbo);
	vbos.push_back(vbo);

	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, data.size()*sizeof(GLfloat), &data[0], storageMode);
	glVertexAttribPointer(attribs, elements, GL_FLOAT, normalize, stride, offset);

	attribs++;
}

void GLArrayBuffer::Attrib3f(float a, float b, float c, bool bind) {
	if (bind) glBindVertexArray(vao);
	
	glEnableVertexAttribArray(attribs);
	glVertexAttrib3f(attribs, a, b, c);

	attribs++;
}
