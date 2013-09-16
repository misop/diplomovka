#include "stdafx.h"
#include "GLProgram.h"
#include <fstream>
#include "Utility.h"

using namespace std;

GLProgram::GLProgram(void)
{
	program = glCreateProgram();
	name = "";
}

GLProgram::GLProgram(std::string programName)
{
	program = glCreateProgram();
	name = programName;
}


GLProgram::~GLProgram(void)
{
	glDeleteProgram(program);
}

GLuint GLProgram::GetProgram() {
	return program;
}

void GLProgram::AttachShader(GLShader *shader) {
	glAttachShader(program, shader->GetShader());
}

bool GLProgram::Link() {
	glLinkProgram(program);

	GLint linked;
	glGetProgramiv(program, GL_LINK_STATUS, &linked);

	return (linked == GL_TRUE);
}

void GLProgram::Use() {
	glUseProgram(program);
}

void GLProgram::BindAttribLocation(GLuint index, string name) {
	glBindAttribLocation(program, index, name.c_str());
}

GLint GLProgram::getUniformLocation(std::string name) {
	GLint location = glGetUniformLocation(program, name.c_str());
	if (location == -1) {
		GLenum err = glGetError();
		string s;
		s = string((char*)gluErrorString(err));
	}
	return location;
}

string GLProgram::GetProgramLog() {
	GLint logLength;
	glGetProgramiv(program, GL_INFO_LOG_LENGTH, &logLength);
	if (logLength == 0) return "";

	GLchar *logCstr = new GLchar[logLength];
	glGetProgramInfoLog(program, logLength, NULL, logCstr);
	string log(logCstr);
	delete [] logCstr;
	return log;
}

void GLProgram::SaveProgramLog() {
	string fileName = "logs/log_program_" + name + ".txt";
	ofstream output(fileName);
	output << GetProgramLog();
	output.close();
}