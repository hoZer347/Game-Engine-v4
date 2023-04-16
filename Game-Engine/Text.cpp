#include "Text.h"

#include "Enums.h"

#include <GLEW/glew.h>
#include <GLFW/glfw3.h>

#include "Data.h"
#include "Utils.h"

#include <ft2build.h>
#include FT_FREETYPE_H

#include <unordered_map>
#include <string>

static inline FT_Library library;

namespace loom
{
    typedef char16_t Letter;
    typedef FT_Face Font;

	static inline Shader shader{ "shaders/default" };
	
	void Text<true>::load()
	{
		FT_Face face;
		FT_New_Face(library, "Resources/alagard.ttf", 0, &face);
		FT_Set_Pixel_Sizes(face, 0, size);

		for (char i : body)
		{
			FT_Load_Char(face, i, FT_LOAD_RENDER);
			glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
			glGenTextures(1, &id);
			glBindTexture(GL_TEXTURE_2D, id);
			glTexImage2D(
				GL_TEXTURE_2D,
				0,
				GL_RED,
				face->glyph->bitmap.width,
				face->glyph->bitmap.rows,
				0,
				GL_RED,
				GL_BYTE,
				face->glyph->bitmap.buffer
			);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
			glBindTexture(GL_TEXTURE_2D, 0);
		};

		FT_Done_Face(face);
	};
	void Text<false>::load()
	{
		static FT_Face face;
		FT_New_Face(library, "Resources/alagard.ttf", 0, &face);
		FT_Set_Pixel_Sizes(face, 0, size);

		for (unsigned char i = 0; i < 128; i++)
		{
			FT_Load_Char(face, i, FT_LOAD_RENDER);
			glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
			glGenTextures(1, &id);
			glBindTexture(GL_TEXTURE_2D, id);
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
			glBindTexture(GL_TEXTURE_2D, 0);
		};

		FT_Done_Face(face);
	};
	
	
	
	void Text<true>::render()
	{
		glUseProgram(shader.id);
		glBindTexture(GL_TEXTURE_2D, id);
		glEnableVertexAttribArray(VEC4_0_32);
		glEnableVertexAttribArray(VEC4_1_32);

		static std::vector<float> data = 
		{
			0, 0, 0, 1,
			0, 0, 0, 1,
			1, 0, 0, 1,
			1, 0, 0, 1,
			1, 1, 0, 1,
			1, 1, 0, 1,
			0, 1, 0, 1,
			0, 1, 0, 1,
		};

		glBufferData(
			GL_ARRAY_BUFFER,
			data.size() * sizeof(float),
			data.data(),
			GL_STREAM_DRAW);

		glDrawArrays(GL_QUADS, 0, 4);

		glDisableVertexAttribArray(VEC4_0_32);
		glDisableVertexAttribArray(VEC4_1_32);
		glBindTexture(GL_TEXTURE_2D, 0);
		glUseProgram(0);
	};
	void Text<false>::render()
	{
		glUseProgram(shader.id);
		glBindTexture(GL_TEXTURE_2D, id);
		glEnableVertexAttribArray(VEC4_0_32);
		glEnableVertexAttribArray(VEC4_1_32);

		static std::vector<float> data =
		{
			0, 0, 0, 1,
			0, 0, 0, 1,
			1, 0, 0, 1,
			1, 0, 0, 1,
			1, 1, 0, 1,
			1, 1, 0, 1,
			0, 1, 0, 1,
			0, 1, 0, 1,
		};

		glBufferData(
			GL_ARRAY_BUFFER,
			data.size() * sizeof(float),
			data.data(),
			GL_STREAM_DRAW);

		glDrawArrays(GL_QUADS, 0, 4);

		glDisableVertexAttribArray(VEC4_0_32);
		glDisableVertexAttribArray(VEC4_1_32);
		glBindTexture(GL_TEXTURE_2D, 0);
		glUseProgram(0);
	};
	
	
	
	void Text<true>::Init() { FT_Init_FreeType(&library); };
	void Text<true>::Exit() { FT_Done_FreeType(library); };
};
