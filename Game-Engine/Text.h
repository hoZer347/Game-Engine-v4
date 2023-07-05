#pragma once

#include <GLEW/glew.h>
#include <GLFW/glfw3.h>

#include "Data.h"
#include "Enums.h"
#include "Camera.h"
#include "Shader.h"

#include <ft2build.h>
#include FT_FREETYPE_H

#include <unordered_map>
#include <string>

#include <glm/glm.hpp>
using namespace glm;

namespace loom
{
	// Letter
	struct Letter final
	{
		vec2 size;
		vec2 bearing;
		vec2 stride;
		vec4 texCoords;

		uint8_t* data;
	};


	// Storage for the info needed to render text
	struct Font final
	{
		~Font();

	protected:
		friend struct ptr<Font>;
		friend struct StaticText;
		friend struct DynamicText;
		Font(const char* font_file, uint32_t&& size = 64);

		uint32_t texture = 0;
		const uint32_t size;
		std::vector<Letter> letters;
		const char* font_file;
	};


	// Static Text
	// - Creates a static texture representing text
	struct StaticText final :
		virtual public Renderable
	{
		mat4 trns = mat4(1);
		mat4& mvp;

		virtual ~StaticText();

	protected:
		friend struct ptr<StaticText>;
		StaticText(ptr<Font> font, std::string&& body, mat4& mvp = Camera::mvp);

	private:
		void render() override;

		ptr<Font> font;
		std::string body;
		static inline std::vector<vec4> vtxs =
		{
			{ 0, 0, 0, 1 },
			{ 1, 0, 0, 1 },
			{ 1, 1, 0, 1 },
			{ 0, 1, 0, 1 },
		};
	};

	struct Option;

	// DynamicText
	// - Creates a text object by using the font's texture
	struct DynamicText final :
		virtual public Renderable
	{
		virtual ~DynamicText();

		mat4 trns{ 1 };
		mat4& mvp = Camera::mvp;
		vec4 color = vec4(0, 0, 0, 1);

		vec4 curr_pos = vec4(0, 0, 0, 1);

		void push(std::string&& body);
		void pop(int amount = 1);
		void clear();

	protected:
		friend struct ptr<DynamicText>;
		friend ptr<Option> TextOption(const ptr<DynamicText> text, Task on_select);
		DynamicText(ptr<Font> font, std::string&& body, mat4& mvp = Camera::mvp);

		void render() override;

		ptr<Font> font;
		std::string body;
		struct Vtx final { vec4 pos, cds; };
		std::vector<Vtx> vtxs;
	};


	struct Text
	{
		static inline ptr<Shader> shader{ 0, "Text" };
		static inline uint32 _mvp;
		static inline uint32 _trns;
		static inline uint32 _color;

		Text()
		{
			Engine::DoOnMain([]()
			{
				_mvp = glGetUniformLocation(shader->id, "mvp");
				_trns = glGetUniformLocation(shader->id, "trns");
				_color = glGetUniformLocation(shader->id, "color");
			});
		};
	};
	static inline Text manager;


	inline Font::Font(const char* font_file, uint32_t&& size) :
		font_file(font_file),
		size(size)
	{
		letters.reserve(128);
		letters.resize(128);

		Engine::DoOnMain([this]()
		{
			// Initializing OpenGL Stuff
			FT_Face face;
			FT_Library library;
			FT_Init_FreeType(&library);
			FT_New_Face(library, this->font_file, 0, &face);
			FT_Set_Pixel_Sizes(face, 0, this->size);
			glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
			glGenTextures(1, &texture);
			glBindTexture(GL_TEXTURE_2D, texture);


			// Extracting info from FT_Face
			uint32_t _w = 2, _h = 2;
			for (unsigned char c = 0; c < 128; c++)
			{
				if (FT_Load_Char(face, c, FT_LOAD_RENDER))
					continue;

				letters[c] =
				{
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
					(uint8_t*)malloc(face->glyph->bitmap.width * face->glyph->bitmap.rows)
				};

				std::memcpy(
					letters[c].data,
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
				_h = this->size * 2,
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
					(GLint)letter.texCoords.x,
					(GLint)letter.texCoords.y,
					(GLsizei)letter.size.x,
					(GLsizei)letter.size.y,
					GL_ALPHA,
					GL_UNSIGNED_BYTE,
					letter.data);

				letter.size /= this->size;
				letter.bearing /= this->size;
				letter.stride /= this->size;
				letter.texCoords /= vec4(_w, _h, _w, _h);
			};
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
			glBindTexture(GL_TEXTURE_2D, 0);
			glPixelStorei(GL_UNPACK_ALIGNMENT, 4);


			// Deallocating Memory
			FT_Done_Face(face);
			FT_Done_FreeType(library);
		});
	};
	inline Font::~Font()
	{
		Engine::DoOnMain([this]()
		{
			// TODO: 
		});
	};


	inline StaticText::StaticText(ptr<Font> font, std::string&& body, mat4& mvp) :
		font(font),
		body(body),
		mvp(mvp)
	{ };
	inline StaticText::~StaticText()
	{ };
	inline void StaticText::render()
	{ };


	inline DynamicText::DynamicText(ptr<Font> font, std::string&& body, mat4& mvp) :
		font(font),
		body(body),
		mvp(mvp)
	{
		Engine::DoOnMain([this]() { push(this->body.c_str()); });
	};
	inline DynamicText::~DynamicText()
	{ };
	inline void DynamicText::push(std::string&& body)
	{
		std::scoped_lock<std::recursive_mutex> lock {GameObject<Renderable>::mut};

		vtxs.reserve(body.size() * 4);
		for (auto c : body)
		{
			Letter& l = font->letters[c];

			if (c == '\n')
			{
				curr_pos.x = 0;
				curr_pos.y += 1;
				continue;
			};

			vtxs.emplace_back(
				curr_pos + vec4(l.bearing.x, l.bearing.y - l.size.y, 0, 0),
				vec4(l.texCoords.x, l.texCoords.w, 0, 1));
			vtxs.emplace_back(
				curr_pos + vec4(l.size.x + l.bearing.x, l.bearing.y - l.size.y, 0, 0),
				vec4(l.texCoords.z, l.texCoords.w, 0, 1));
			vtxs.emplace_back(
				curr_pos + vec4(l.size.x + l.bearing.x, l.bearing.y, 0, 0),
				vec4(l.texCoords.z, l.texCoords.y, 0, 1));
			vtxs.emplace_back(
				curr_pos + vec4(l.bearing.x, l.bearing.y, 0, 0),
				vec4(l.texCoords.x, l.texCoords.y, 0, 1));

			curr_pos.x += l.stride.x;
		};

		this->body += body;
	};
	inline void DynamicText::pop(int amount)
	{
		std::scoped_lock<std::recursive_mutex> lock {GameObject<Renderable>::mut};

		for (auto i = 0; i < amount; i++)
		{
			curr_pos.x -= font->letters[body.back()].stride.x;
			body.pop_back();
		};

		vtxs.resize(vtxs.size() - 4 * amount);
	};
	inline void DynamicText::clear()
	{
		std::scoped_lock<std::recursive_mutex> lock {GameObject<Renderable>::mut};

		vtxs.clear();
		body.clear();
	};
	inline void DynamicText::render()
	{
		glUseProgram(Text::shader->id);
		glBindTexture(GL_TEXTURE_2D, font->texture);

		glEnableVertexAttribArray(VEC4_0_32);
		glEnableVertexAttribArray(VEC4_1_32);

		glUniformMatrix4fv(Text::_mvp, 1, GL_FALSE, &mvp[0][0]);
		glUniformMatrix4fv(Text::_trns, 1, GL_FALSE, &trns[0][0]);
		glUniform4fv(Text::_color, 1, &color[0]);

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
};
