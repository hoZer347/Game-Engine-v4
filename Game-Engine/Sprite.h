#pragma once

#include "Data.h"
#include "Enums.h"

#include <glm/glm.hpp>
using namespace glm;

namespace loom
{
	struct Texture;

	struct Sprite : GameObject<Sprite>
	{
		Sprite(Texture& texture, vec2 start, vec2 size, vec2 move, uint16_t ups);
		
		mat4 trns{ 1 };
		mat4& mvp;

		static inline const int collision_type = COLLISION::SQUARE;
		
		vec4 location;

		vec2 stride;
		uint32 ups;

		Texture& texture;
	};
};
