#pragma once

#include "Data.h"

#include <glm/glm.hpp>
using namespace glm;

namespace loom
{
	struct Sprite :
		virtual protected Loadable
	{
		Sprite(Texture& texture, vec2 start, vec2 size, vec2 move, uint16_t ups);
		
		Mat4 trns;
		Mat4 mvp;

		static inline const int collision_type = COLLISION::SQUARE;
		
	protected:
		friend struct SpriteManager;
		friend struct Geometry<Sprite>;
		vec4 location;

	private:
		void load() override;

		Texture& texture;

		vec2 stride;
		uint32_t ups;
	};

	struct Sprites
	{
		// TODO: Implement
	};
};
