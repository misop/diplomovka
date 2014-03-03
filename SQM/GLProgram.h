#pragma once
#include "GLShader.h"
#include "Uniforms.h"

class GLProgram
{
private:
public:
	GLuint program;
	std::string name;
	
	GLProgram(void);
	GLProgram(std::string programName);
	~GLProgram(void);

	GLuint GetProgram();
	void AttachShader(GLShader *shader);
	void AttachShaders(OpenGLShaders *shaders);
	bool Link();
	void Use();
	void BindAttribLocation(GLuint index, std::string name);
	GLint getUniformLocation(std::string name);
	std::string GetProgramLog();
	void SaveProgramLog();
};

