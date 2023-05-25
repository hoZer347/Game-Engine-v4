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
	static inline Shader shader{ "shaders/Text" };
	static inline uint32_t _mvp;
	static inline uint32_t _trns;

	Font::Font(const char*&& font_file, uint32_t&& size) :
		font_file(font_file),
		size(size)
	{ };
	void Font::load()
	{
		// Initializing OpenGL Stuff
		FT_Face face;
		FT_Library library;
		FT_Init_FreeType(&library);
		FT_New_Face(library, font_file, 0, &face);
		FT_Set_Pixel_Sizes(face, 0, size);
		glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

		glGenTextures(1, &texture);
		glBindTexture(GL_TEXTURE_2D, texture);
		_mvp = glGetUniformLocation(shader.id, "mvp");
		_trns = glGetUniformLocation(shader.id, "trns");

		// Extracting info from FT_Face
		uint32_t _w=2, _h=2;
		letters.reserve(128);
		for (unsigned char c = 0; c < 128; c++)
		{
			if (FT_Load_Char(face, c, FT_LOAD_RENDER))
			{
				letters.emplace_back();
				continue;
			};

			letters.emplace_back(
				vec2{
					face->glyph->bitmap.width,
					face->glyph->bitmap.rows, },
				vec2{
					face->glyph->bitmap_left,
					face->glyph->bitmap_top, },
				vec2{
					face->glyph->advance.x >> 6,
					face->glyph->advance.y >> 6, },
				vec4{
					_w,
					_h,
					_w + face->glyph->bitmap.width,
					_h + face->glyph->bitmap.rows + 2, },
				(uint8_t*)malloc(face->glyph->bitmap.width * face->glyph->bitmap.rows));
			
			std::memcpy(
				letters.back().data,
				face->glyph->bitmap.buffer,
				face->glyph->bitmap.width * face->glyph->bitmap.rows);

			_w += face->glyph->bitmap.width + 1;
		};


		// Setting Dimensions of Texture
		glTexImage2D(
			GL_TEXTURE_2D,
			0,
			GL_ALPHA,
			_w,
			_h = size * 2,
			0,
			GL_ALPHA,
			GL_UNSIGNED_BYTE,
			nullptr);


		// Assigning Locations
		for (auto& letter : letters)
		{
			if (letter.size.x < 0)
				continue;

			glTexSubImage2D(
				GL_TEXTURE_2D,
				0,
				letter.texCoords.x,
				letter.texCoords.y,
				letter.size.x,
				letter.size.y,
				GL_ALPHA,
				GL_UNSIGNED_BYTE,
				letter.data);

			letter.size			/= size;
			letter.bearing		/= size;
			letter.stride		/= size;
			letter.texCoords	/= vec4(_w, _h, _w, _h);
		};

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		glBindTexture(GL_TEXTURE_2D, 0);
		glPixelStorei(GL_UNPACK_ALIGNMENT, 4);

		// Deallocating Memory
		FT_Done_Face(face);
		FT_Done_FreeType(library);
	};
	void Font::unload()
	{

	};



	StaticText::StaticText(Font& font, std::string&& body) :
		font(font),
		body(body)
	{ };
	StaticText::~StaticText()
	{ };
	void StaticText::load()
	{ };
	void StaticText::render()
	{ };



	DynamicText::DynamicText(Font& font, std::string&& body) :
		font(font),
		body(body)
	{ };
	DynamicText::~DynamicText()
	{ };
	void DynamicText::push(std::string&& body)
	{
		vtxs.reserve(body.size() * 4);
		for (auto c : body)
		{
			Letter& l = font.letters[c];

			vtxs.emplace_back(
				curr_pos + vec4(l.bearing.x, l.bearing.y - l.size.y, 0, 1),
				vec4(l.texCoords.x, l.texCoords.w, 0, 1));
			vtxs.emplace_back(
				curr_pos + vec4(l.size.x + l.bearing.x, l.bearing.y - l.size.y, 0, 1),
				vec4(l.texCoords.z, l.texCoords.w, 0, 1));
			vtxs.emplace_back(
				curr_pos + vec4(l.size.x + l.bearing.x, l.bearing.y, 0, 1),
				vec4(l.texCoords.z, l.texCoords.y, 0, 1));
			vtxs.emplace_back(
				curr_pos + vec4(l.bearing.x, l.bearing.y, 0, 1),
				vec4(l.texCoords.x, l.texCoords.y, 0, 1));

			this->body += body;

			curr_pos.x += l.stride.x;
		};
	};
	void DynamicText::pop(uint32_t amount)
	{
		vtxs.resize(-4 * amount);
	};
	void DynamicText::load()
	{
		push(body.c_str());
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
			vtxs.size() * sizeof(Vtx),
			vtxs.data(),
			GL_STATIC_DRAW);
		
		glDrawArrays(GL_QUADS, 0, (GLsizei)vtxs.size());
		
		glDisableVertexAttribArray(VEC4_0_32);
		glDisableVertexAttribArray(VEC4_1_32);
		
		glBindTexture(GL_TEXTURE_2D, 0);
		glUseProgram(0);
	};
	void DynamicText::clear()
	{
		vtxs.clear();
		body.clear();
	};
};
