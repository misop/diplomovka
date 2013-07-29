#pragma once
#include <GL\glew.h>
#include <GL\wglew.h>
#include <string>

class GLShader
{
private:
	GLuint shader;

	std::string readShaderFile(std::string fileName);
public:
	std::string name;
	
	GLShader(GLenum shaderType, std::string shaderName);
	GLShader(GLenum shaderType);
	~GLShader(void);

	GLuint GetShader();
	void Load(std::string shaderFileName);
	bool Compile();
	std::string GetShaderLog();
	void SaveShaderLog();
};

