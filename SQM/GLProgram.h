#pragma once
#include "GLShader.h"

#define MVP_MATRIX_STR "MVPmatrix"
#define MODEL_MATRIX_STR "ModelMatrix"
#define MODEL_VIEW_MATRIX_STR "ModelViewMatrix"
#define NORMAL_MATRIX_STR "NormalMatrix"

#define TESS_LEVEL_INNER_STR "TessLevelInner"
#define TESS_LEVEL_OUTER_STR "TessLevelOuter"

#define AMBIENT_COLOR_STR "AmbientColor"
#define DIFFUSE_COLOR_STR "DiffuseColor"
#define SPECULAR_COLOR_STR "SpecularColor"
#define LIGHT_POSITION_STR "LightPosition"

#define WIREFRAME_STR "Wireframe"
#define THRESHOLD_STR "Threshold"

#define RADIUS_TEXTURE_STR "RadiusesSampler"

class GLProgram
{
private:
	GLuint program;
public:
	std::string name;
	
	GLProgram(void);
	GLProgram(std::string programName);
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

