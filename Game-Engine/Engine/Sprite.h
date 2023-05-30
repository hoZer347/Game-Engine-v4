#pragma once

#include "Data.h"

#include <glm/glm.hpp>
using namespace glm;

namespace loom
{
	struct Sprite final : public Loadable, public GameObject<Sprite>
	{
		Sprite(Texture& texture, vec2 start, vec2 size, vec2 move, uint16_t ups);
		
		mat4 trns = mat4(1);

	protected:
		friend struct SpriteManager;
		vec4 location;

	private:
		void load() override;
		Texture& texture;

		vec2 stride;
		uint32_t ups;
	};
};
