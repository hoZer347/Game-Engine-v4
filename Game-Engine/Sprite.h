#pragma once

#include "Data.h"
#include "Enums.h"

#include <glm/glm.hpp>
using namespace glm;

namespace loom
{
	struct Sprite :
		public GameObject<Sprite>,
		virtual public Loadable
	{
		Sprite(Texture& texture, vec2 start, vec2 size, vec2 move, uint16_t ups);
		
		mat4 trns{ 1 };
		mat4& mvp;

		static inline const int collision_type = COLLISION::SQUARE;
		
	protected:
		friend struct SpriteManager;
		static inline ptr<Shader> shader{ "Sprite" };
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
