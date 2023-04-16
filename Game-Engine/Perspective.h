#pragma once

#include <glm/glm.hpp>
using namespace glm;

namespace loom
{
	struct Perspective final
	{
		void load();
		void update();
		void render();

		mat4 trns = mat4(1);

	protected:
		friend struct Utils;
		Perspective()
		{ };
	};
};
