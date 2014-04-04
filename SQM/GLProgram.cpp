#include "stdafx.h"
#include "GLProgram.h"
#include <fstream>
#include "Utility.h"
#include <boost\filesystem.hpp>

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

void GLProgram::AttachShaders(OpenGLShaders *shaders) {
	if (shaders->vert) glAttachShader(program, shaders->vert->GetShader());
	if (shaders->ctrl) glAttachShader(program, shaders->ctrl->GetShader());
	if (shaders->eval) glAttachShader(program, shaders->eval->GetShader());
	if (shaders->geom) glAttachShader(program, shaders->geom->GetShader());
	if (shaders->frag) glAttachShader(program, shaders->frag->GetShader());
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
	string log = GetProgramLog();
	string fileName = "logs/log_program_" + name + ".txt";
	if (!boost::filesystem::exists(fileName) && log.size() == 0) return;

	ofstream output(fileName);
	output << log;
	output.close();
}