#pragma once
#include <GL\glew.h>
#include <GL\wglew.h>
#include <string>
#include <map>
#include <vector>

using namespace std;

class GLTexture
{
public:
	GLuint texture;
	GLenum target;

	GLTexture(GLenum texType);
	~GLTexture(void);

	void Enable();
	void Disable();
	void Bind();
	void Unbind();
	void UseTexture(GLint loc, int unit);
	void TexParameteri(GLenum pname, GLint param);
	
	void Texture2D(int width, int height, GLenum internalFormat, GLenum format, GLenum type, GLubyte *data);
	void FunctionTexture(int width, int height, float *data);
	void RGBATexture(int width, int height, GLenum format, GLenum type, GLubyte *data);
	void DepthTexture(int width, int height, GLenum type);
	
	void LoadRGBATextureFromImage(string img);
	void LoadCubeTextureFromImages(string posxFile, string negxFile, string posyFile, string negyFile, string poszFile, string negzFile);
};

