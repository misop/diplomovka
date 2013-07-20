#pragma once
#include <string>
#include <GL\glew.h>
#include <GL\wglew.h>
#include "GLProgram.h"
#include "GLShader.h"

struct ShaderUniforms {
	GLint MVPmatrixSklLines;
	GLint MVPmatrixSklNodes;
	GLint ModelMatrix;
	GLint SelectedNodeLoc;
	GLint TessLevelInner;
	GLint TessLevelOuter;
	GLint CameraLoc;
};

struct OpenGLPrograms {
	GLProgram *SklLines;
	GLProgram *SklNodes;

	OpenGLPrograms() : SklLines(NULL), SklNodes(NULL) { }
	~OpenGLPrograms() {
		if (SklLines) delete SklLines;
		if (SklNodes) delete SklNodes;
	}
};

struct OpenGLShaders {
	GLShader *vert;
	GLShader *ctrl;
	GLShader *eval;
	GLShader *geom;
	GLShader *frag;

	OpenGLShaders() : vert(NULL), ctrl(NULL), eval(NULL), geom(NULL), frag(NULL) { }
	~OpenGLShaders() {
		if (vert) delete vert;
		if (ctrl) delete ctrl;
		if (eval) delete eval;
		if (geom) delete geom;
		if (frag) delete frag;
	}
};

template <typename T> std::string ToString(T& a);