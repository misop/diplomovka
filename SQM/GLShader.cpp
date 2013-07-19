#include "stdafx.h"
#include "GLShader.h"
#include <fstream>
#include <iostream>
#include <sstream>
#include "Utility.cpp"

using namespace std;

GLShader::GLShader(GLenum shaderType)
{
	shader = glCreateShader(shaderType);
}

GLShader::~GLShader(void)
{
	glDeleteShader(shader);
	shader = 0;
}

GLuint GLShader::GetShader() {
	return shader;
}

std::string GLShader::readShaderFile(std::string fileName) {
	ifstream input(fileName);
	stringstream buffer;
	buffer << input.rdbuf();
	input.close();

	return buffer.str();
}

void GLShader::Load(std::string shaderFileName) {
	string shaderSource = readShaderFile(shaderFileName);
	GLint length = shaderSource.length();
	const char *shaderSourceCstr = shaderSource.c_str();

	glShaderSource(shader, 1, &shaderSourceCstr, &length);
}

bool GLShader::Compile() {
	glCompileShader(shader);

	GLint compiled;
	glGetShaderiv(shader, GL_COMPILE_STATUS, &compiled);

	return (compiled == GL_TRUE);
}

std::string GLShader::GetShaderLog() {
	GLint logLength;
	glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &logLength);
	if (logLength == 0) return "";

	GLchar *logCstr = new GLchar[logLength];
	glGetShaderInfoLog(shader, logLength, NULL, logCstr);
	string log(logCstr);
	delete [] logCstr;

	return log;
}

void GLShader::SaveShaderLog() {
	string fileName = "log_shader_" + ToString(shader) + ".txt";
	ofstream output(fileName);
	output << GetShaderLog();
	output.close();
}