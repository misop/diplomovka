#pragma once
#include <string>
#include <GL\glew.h>
#include <GL\wglew.h>

struct ShaderUniforms {
	GLint MVPmatrixLoc;
	GLint ModelMatrixLoc;
	GLint SelectedNodeLoc;
	GLint TessLevelInner;
	GLint TessLevelOuter;
};

template <typename T> std::string ToString(T& a);