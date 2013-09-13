#include "stdafx.h"
#include "GLShader.h"
#include <fstream>
#include <iostream>
#include <sstream>
#include "Utility.h"
//#include "Utility.cpp"

using namespace std;

GLShader::GLShader(GLenum shaderType)
{
	shader = glCreateShader(shaderType);
	name = "";
}

GLShader::GLShader(GLenum shaderType, std::string shaderName)
{
	shader = glCreateShader(shaderType);
	name = shaderName;
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
	name = shaderFileName;
	string shaderSource = readShaderFile(shaderFileName);
	GLint length = shaderSource.length();
	const char *shaderSourceCstr = shaderSource.c_str();

	glShaderSource(shader, 1, &shaderSourceCstr, &length);
}

void GLShader::Load(std::string shaderFileName, std::map<std::string, std::string> replaceMap) {
	string shaderSource = readShaderFile(shaderFileName);

	for(map<string, string>::iterator it = replaceMap.begin(); it != replaceMap.end(); it++) {
		string key = it->first;
		string value = it->second;
		size_t found = shaderSource.find(key);

		while (found != string::npos) {
			shaderSource.replace(found, key.length(), value);

			found = shaderSource.find(key);
		}
	}

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
	string fileName = "logs/log_shader_" + name + ".txt";
	ofstream output(fileName);
	output << GetShaderLog();
	output.close();
}