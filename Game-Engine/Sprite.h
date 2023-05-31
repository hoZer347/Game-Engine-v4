#pragma once

#include "Data.h"

#include <glm/glm.hpp>
using namespace glm;

namespace loom
{
	struct Sprite final :
		virtual protected Loadable,
		public GameObject<Sprite>
	{
		Sprite(Texture& texture, vec2 start, vec2 size, vec2 move, uint16_t ups);
		
		mat4 trns = mat4(1);
		
		static inline const int collision_type = COLLISION::SQUARE;
		
	protected:
		friend struct SpriteManager;
		friend struct Geometry<Sprite>;
		static inline std::vector<vec4> vtxs
		{
			vec4(0, 0, 0, 1),
			vec4(1, 0, 0, 1),
			vec4(1, 0, 1, 1),
			vec4(0, 0, 1, 1),
		};
		vec4 location;

	private:
		void load() override;
		Texture& texture;

		vec2 stride;
		uint32_t ups;
	};
};
