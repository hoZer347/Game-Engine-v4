#pragma once

#include "Data.h"

#include <glm/glm.hpp>
using namespace glm;

namespace loom
{
	// Storage for the info needed to render text
	struct Font final :
		virtual protected Loadable,
		virtual protected Unloadable
	{
		Font(const char*&& font_file, uint32_t&& size = 64);

	protected:
		friend struct StaticText;
		friend struct DynamicText;
		uint32_t texture = 0;
		std::vector<vec4> vtxs =
		{
			{ 0, 0, 0, 1 }, { 0, 0, 0, 1 },
			{ 1, 0, 0, 1 }, { 1, 0, 0, 1 },
			{ 1, 1, 0, 1 }, { 1, 1, 0, 1 },
			{ 0, 1, 0, 1 }, { 0, 1, 0, 1 },
		};

	private:
		void load() override;
		void unload() override;

		const char* font_file;
		uint32_t size;
	};
	//



	// Static Text
	// - Creates a static texture representing text
	struct StaticText final : virtual protected Renderable
	{
		StaticText(Font& font, std::string&& body);
		virtual ~StaticText();

		mat4 trns = mat4(1);

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
	//



	// DynamicText
	// - Creates a text object by using the font's texture
	struct DynamicText final : virtual protected Renderable
	{
		DynamicText(Font& font, std::string&& body);
		virtual ~DynamicText();

		mat4 trns = mat4(1);

		void push(std::string&& s);
		void pop();
		void clear();

	private:
		void render() override;

		Font& font;
		std::string body;
	};
	//
};
