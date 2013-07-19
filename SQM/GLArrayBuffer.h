#pragma once
#include <GL\glew.h>
#include <GL\wglew.h>
#include <string>
#include <vector>

class GLArrayBuffer
{
	GLuint vao;
	std::vector<GLuint> vbos;
	GLint attribs;
	int vertices;
	std::vector<int> indices;
	std::vector<GLuint> elementBuffers;
public:
	GLArrayBuffer(void);
	~GLArrayBuffer(void);

	void Bind();
	void Draw(GLenum mode, bool bind = true);
	void DrawElement(int element, GLenum mode, bool bind = true);

	void BindBufferData(std::vector<float> &data, GLint elements, GLenum storageMode = GL_DYNAMIC_DRAW, GLboolean normalize = GL_FALSE, GLsizei stride = 0, GLvoid* offset = NULL, bool bind = false);
	void Attrib3f(float a, float b, float c, bool bind = false);

	int BindElement(std::vector<int> &data, GLenum storageMode = GL_DYNAMIC_DRAW);
};

