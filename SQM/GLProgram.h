#pragma once
#include "GLShader.h"

struct ProgramUniforms {
	GLint MVPmatrix;
	GLint ModelMatrix;
	GLint ModelViewMatrix;
	GLint NormalMatrix;

	GLint SpecularColor;
	GLint TessLevelInner;
	GLint TessLevelOuter;

	GLint AmbientColor;
	GLint DiffuseColor;
	GLint LightPosition;
};

class GLProgram
{
private:
	GLuint program;
public:
	ProgramUniforms uniforms;

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

