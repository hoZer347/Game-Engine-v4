#pragma once

#include "Data.h"
#include "Enums.h"
#include "Camera.h"
#include "Shader.h"
#include "Texture.h"
#include "Control.h"
#include "Geometry.h"

#include <GLEW/glew.h>
#include <GLFW/glfw3.h>

#include <glm/gtx/transform.hpp>

#include <vector>

namespace loom
{
	namespace sprite
	{
		inline mat4* _mvp = &Camera::mvp;
		inline mat4* _trns;
		inline vec4* _location;
		inline Shader shader{ "Sprite", "loc", &_location, "trns", &_trns, "mvp", &_mvp};

		struct Sprite : geo::Rect<>
		{
			Sprite(Texture& texture, vec2 start, vec2 size, vec2 stride, uint16_t ups) :
				geo::Rect<>(sprite::shader),
				texture(texture),
				location(start, size),
				stride(stride),
				ups(ups)
			{
				this->modify(0,
					vec4(0, 0, 0, 1),
					vec4(1, 0, 0, 1),
					vec4(1, 0, 1, 1),
					vec4(0, 0, 1, 1));
				this->index(0,
					0, 1, 2, 3);

				Engine::DoOnMain([this, &texture]()
				{
					location.x /= texture.w;
					location.y /= texture.h;
					location.z /= texture.w;
					location.w /= texture.h;

					glBindTexture(GL_TEXTURE_2D, texture.id);
					glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
					glBindTexture(GL_TEXTURE_2D, 0);
				});
			};
			mat4 trns{ 1 };

			static inline const int collision_type = COLLISION::SQUARE;

			vec4 location;

			vec2 stride;
			uint32 ups;

			Texture& texture;

			inline void subRender() override
			{
				glBindTexture(GL_TEXTURE_2D, texture.id);
				sprite::_trns = &trns;
 				sprite::_location = &location;
			};
		};
	};
};
