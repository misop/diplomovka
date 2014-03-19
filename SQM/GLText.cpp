#include "stdafx.h"
#include "GLText.h"
#define generic GenericFromFreeTypeLibrary
#include <freetype\ft2build.h>
#include <freetype/freetype.h>
#include <freetype/ftglyph.h>
#include <freetype/ftoutln.h>
#include <freetype/fttrigon.h>
#undef generic
#include <gtc\matrix_transform.hpp>
#include <gtc\type_ptr.hpp>
#include "Uniforms.h"


GLText::GLText(string fileName) {
	FT_Library library;
	if (FT_Init_FreeType(&library)) return;

	FT_Face face;
	if (FT_New_Face(library, fileName.c_str(), 0, &face)) {
		FT_Done_FreeType(library);
		return;
	}

	int fontHeight = 16;
	FT_Set_Char_Size(face, 64 * fontHeight, 64 * fontHeight, 96, 96);
	
	GLfloat verts[] = {0.0,0.0, 1.0,0.0, 1.0,1.0, 0.0,1.0};
	
	vector<float> vertices(std::begin(verts), std::end(verts));

	vao = new GLArrayBuffer();
	vao->Bind();
	vao->BindBufferDataf(vertices, 2, GL_STATIC_DRAW);
	glBindVertexArray(0);

	glGenTextures(96, characters);
	vector<char> test;
	for (int i = 0; i < 96; i++) {
		// inicializuje bitmapu pre aktualny glyf
		if (FT_Load_Glyph(face, FT_Get_Char_Index(face, i + 32), FT_LOAD_DEFAULT)) continue;
		FT_Glyph glyph;
		if(FT_Get_Glyph(face->glyph, &glyph)) continue;

		FT_Glyph_To_Bitmap(&glyph, ft_render_mode_normal, 0, 1);
		FT_BitmapGlyph bitmap_glyph = (FT_BitmapGlyph)glyph;
		FT_Bitmap& bitmap = bitmap_glyph->bitmap;

		// uloz potrebne data pre glyf
		GlyphMetrics glyphMetrics;
		glyphMetrics.top = bitmap_glyph->top;
		glyphMetrics.left = bitmap_glyph->left;
		glyphMetrics.advance = face->glyph->advance.x >> 6;
		glyphMetrics.bitmap_width = bitmap.width;
		glyphMetrics.bitmap_height = bitmap.rows;
		glyphs[i] = glyphMetrics;

		// ak nemame bitmapu pre glyf, preskoc glyf
		if (bitmap.width == 0 || bitmap.rows == 0) continue;

		// vypocitaj rozmery textury, mocniny 2
		int iNX = int(log((float)bitmap.width)/log(2.0)) + 1;
		int texture_width = int(pow(2.0, iNX));
		int iNY = int(log((float)bitmap.rows)/log(2.0)) + 1;
		int texture_height = int(pow(2.0, iNY));

		// rozsir bitmapu do textury
		GLubyte* texture_data = new GLubyte[ 2 * texture_width * texture_height];
		for(int j = 0; j < texture_height; j++) {
			for(int k = 0; k < texture_width; k++) {
				int bitmap_x = int((bitmap.width - 1) * k / float(texture_width - 1));
				int bitmap_y = int((bitmap.rows - 1) * j / float(texture_height - 1));
				texture_data[2*(k + j*texture_width)] = texture_data[2*(k + j*texture_width)+1] = bitmap.buffer[bitmap_x + bitmap.width*bitmap_y];
			}
		}

		// nacitaj texturu do opengl
		glBindTexture(GL_TEXTURE_2D, characters[i]);
		glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, texture_width, texture_height, 0, GL_LUMINANCE_ALPHA, GL_UNSIGNED_BYTE, texture_data);
		delete [] texture_data;

		float bitmap_width = glyphs[i].bitmap_width;
		float bitmap_height = glyphs[i].bitmap_height;
		float offset_x = glyphs[i].left;
		float offset_y = glyphs[i].top - bitmap_height;
		
		glm::mat4 scale = glm::scale(glm::mat4(1.0), glm::vec3(bitmap_width, bitmap_height, 1.0));
		glm::mat4 translate = glm::translate(glm::mat4(1.0), glm::vec3(offset_x, offset_y, 0));
		models[i] = translate * scale;
	}

	FT_Done_Face(face);
	FT_Done_FreeType(library);
}


GLText::~GLText(void)
{
	glDeleteTextures(96, characters);
	delete vao;
}

void GLText::RenderText(float x, float y, string text, GLCamera *camera) {
	float current_x = x;
	float current_y = y;
	
	glm::mat4 proj = glm::ortho(0.0f, camera->width, 0.0f, camera->height, -1.0f, 1.0f);
	glUniformMatrix4fv(PROJECTION_MATRIX, 1, GL_FALSE, glm::value_ptr(proj));

	for (int i = 0; i < text.size(); i++) {
		char c = text[i];
		glm::mat4 model = glm::translate(glm::mat4(1.0), glm::vec3(current_x, current_y, 0));
		model = model * models[c - 32];
		if (glyphs[c - 32].bitmap_width > 0 && glyphs[c - 32].bitmap_height > 0) {
			glUniformMatrix4fv(MODEL_MATRIX, 1, GL_FALSE, glm::value_ptr(model));
			glActiveTexture(GL_TEXTURE0);
			glBindTexture(GL_TEXTURE_2D, characters[c - 32]);

			vao->Bind();
			vao->Draw(GL_QUADS);
		}
		
		current_x += glyphs[c - 32].advance;
	}
}