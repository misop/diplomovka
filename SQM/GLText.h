#pragma once
#include <string>
#include <GL\glew.h>
#include <GL\wglew.h>
#include <glm.hpp>
#include "GLCamera.h"
#include "GLArrayBuffer.h"
using namespace std;

struct GlyphMetrics
{
    int left;
    int top;
    int advance;
    int bitmap_width;
    int bitmap_height;
};

class GLText
{
	GLArrayBuffer *vao;
	GLuint characters[96];
	glm::mat4 models[96];
	GlyphMetrics glyphs[96];
public:
	float fontSize;
	glm::vec4 fontColor;

	GLText(string fileName);
	~GLText(void);

	void RenderText(float x, float y, string text, GLCamera *camera);
};

