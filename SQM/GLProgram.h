#pragma once
#include "GLShader.h"

class GLProgram
{
private:
	GLuint program;
public:
	GLProgram(void);
	~GLProgram(void);

	GLuint GetProgram();
	void AttachShader(GLShader *shader);
	bool Link();
	void Use();
	void BindAttribLocation(GLuint index, std::string name);
	GLint getUniformLocation(std::string name);
	std::string GetProgramLog();
	void SaveProgramLog();
};

