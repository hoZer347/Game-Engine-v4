#pragma once

#include "Data.h"

#include <glm/glm.hpp>
using namespace glm;

#include <string>

namespace loom
{
	// TODO: Include static / dynamic Text options
	
	
	template <bool STATIC = true>
	struct Text final : public Renderable, public Loadable
	{
		Text(const char* text = "Unknown", const char* font = "Resources/alagard.ttf", unsigned int size = 48)
		: body(body), font(font), size(size)
		{ };

		mat4 trns = mat4(1);

	protected:
		friend struct Loom;
		static void Init();
		static void Exit();

	private:
		void load() override;
		void render() override;

		std::string body;
		const char* font;
		unsigned int size;
		uint32_t id=0;
	};
};
