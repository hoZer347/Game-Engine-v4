#pragma once


#include <glm/glm.hpp>
using namespace glm;


#include "Data.h"
#include "Camera.h"


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
		Font(const char*&& font_file, uint32_t&& size = 64);
		~Font();

	protected:
		friend struct StaticText;
		friend struct DynamicText;
		uint32_t texture = 0;
		const uint32_t size;

	private:

		std::vector<Letter> letters;
		const char* font_file;
	};


	// Static Text
	// - Creates a static texture representing text
	struct StaticText final :
		virtual public Renderable
	{
		StaticText(Font& font, std::string&& body);
		virtual ~StaticText();

		mat4 trns = mat4(1);
		mat4& mvp = Camera::mvp;

	private:
		void render() override;

		Font& font;
		std::string body;
		static inline std::vector<vec4> vtxs =
		{
			{ 0, 0, 0, 1 },
			{ 1, 0, 0, 1 },
			{ 1, 1, 0, 1 },
			{ 0, 1, 0, 1 },
		};
	};


	// DynamicText
	// - Creates a text object by using the font's texture
	struct DynamicText final :
		virtual public Renderable
	{
		DynamicText(Font& font, std::string&& body);
		virtual ~DynamicText();

		mat4 trns = mat4(1);
		mat4& mvp = Camera::mvp;

		void push(std::string&& body);
		void pop(uint32_t amount=1);
		void clear();

	private:
		void render() override;

		Font& font;
		std::string body;
		vec4 curr_pos = vec4(0, 0, 0, 0);
		struct Vtx final { vec4 pos, cds; };
		std::vector<Vtx> vtxs;
	};
};
