#include "Text.h"

#include "Enums.h"

#include <GLEW/glew.h>
#include <GLFW/glfw3.h>

#include "Data.h"
#include "Utils.h"
#include "Camera.h"

#include <ft2build.h>
#include FT_FREETYPE_H

#include <unordered_map>
#include <string>

namespace loom
{
	static inline Shader shader{ "shaders/default" };
	static inline uint32_t _mvp;
	static inline uint32_t _trns;

	struct Letter final
	{
		ivec2 size;
		ivec2 offset;
		ivec2 stride;

		uint32_t texture;
	};

	Font::Font(const char*&& font_file, uint32_t&& size) :
		font_file(font_file),
		size(size)
	{ };
	void Font::load()
	{
		FT_Face face;
		FT_Library library;
		FT_Init_FreeType(&library);
		FT_New_Face(library, font_file, 0, &face);
		FT_Set_Pixel_Sizes(face, 0, size);
		
		glGenTextures(1, &texture);
		glBindTexture(GL_TEXTURE_2D, texture);

		glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
		for (unsigned char c = 0; c < 128; c++)
		{
			// load character glyph
			if (FT_Load_Char(face, c, FT_LOAD_RENDER)) continue;
			// generate texture
			glTexImage2D(
				GL_TEXTURE_2D,
				0,
				GL_RED,
				face->glyph->bitmap.width,
				face->glyph->bitmap.rows,
				0,
				GL_RED,
				GL_UNSIGNED_BYTE,
				face->glyph->bitmap.buffer
			);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		};

		_mvp = glGetUniformLocation(shader.id, "mvp");
		_trns = glGetUniformLocation(shader.id, "trns");

		FT_Done_Face(face);
		FT_Done_FreeType(library);
	};
	void Font::unload()
	{

	};



	StaticText::StaticText(Font& font, std::string&& body) :
		font(font),
		body(body)
	{

	};
	StaticText::~StaticText()
	{

	};
	void StaticText::render()
	{

	};



	DynamicText::DynamicText(Font& font, std::string&& body) :
		font(font),
		body(body)
	{

	};
	DynamicText::~DynamicText()
	{

	};
	void DynamicText::push(std::string&& s)
	{

	};
	void DynamicText::pop()
	{

	};
	void DynamicText::render()
	{
		glUseProgram(shader.id);
		glBindTexture(GL_TEXTURE_2D, font.texture);

		glEnableVertexAttribArray(VEC4_0_32);
		glEnableVertexAttribArray(VEC4_1_32);
		
		glUniformMatrix4fv(_mvp, 1, GL_FALSE, &Camera::mvp[0][0]);
		glUniformMatrix4fv(_trns, 1, GL_FALSE, &trns[0][0]);
		
		glBufferData(
			GL_ARRAY_BUFFER,
			sizeof(vec4) * font.vtxs.size(),
			font.vtxs.data(),
			GL_STATIC_DRAW);
		
		glBufferData(
			GL_ELEMENT_ARRAY_BUFFER,
			sizeof(uint32_t) * inds.size(),
			inds.data(),
			GL_STATIC_DRAW);
		
		glDrawElements(GL_QUADS, (GLsizei)inds.size(), GL_UNSIGNED_INT, nullptr);
		
		glDisableVertexAttribArray(VEC4_0_32);
		glDisableVertexAttribArray(VEC4_1_32);
		
		glBindTexture(GL_TEXTURE_2D, 0);
		glUseProgram(0);
	};
	void DynamicText::clear()
	{

	};
};
