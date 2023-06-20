#pragma once

#include "Data.h"
#include "Enums.h"
#include "Camera.h"
#include "Shader.h"
#include "Texture.h"
#include "Control.h"

#include <GLEW/glew.h>
#include <GLFW/glfw3.h>

#include <glm/gtx/transform.hpp>

#include <vector>

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

		static inline uint32 _mvp;
		static inline uint32 _trns;
		static inline uint32 _location;
		static inline Shader shader{"Sprite"};
	};

	struct SpriteManager :
		virtual public Updateable,
		virtual public Renderable
	{
		SpriteManager()
		{
			Engine::DoOnMain([]()
			{

			});

			Engine::Add(this);
		};

		static inline std::vector<vec4> vtxs
		{
			vec4(0, 0, 0, 1),
			vec4(1, 0, 0, 1),
			vec4(1, 0, 1, 1),
			vec4(0, 0, 1, 1),
		};

	private:
		inline void update() override
		{
			GameObject<Sprite>::access([](auto& sprite)
			{

			});
		};
		inline void render() override
		{
			Sprite::shader.load();
			glEnableVertexAttribArray(VEC4_0_16);
			glUniformMatrix4fv(Sprite::_mvp, 1, GL_FALSE, &Camera::mvp[0][0]);
			glBufferData(GL_ARRAY_BUFFER, vtxs.size() * sizeof(vec4), vtxs.data(), GL_STATIC_DRAW);

			GameObject<Sprite>::access([](auto& sprite)
			{
				glBindTexture(GL_TEXTURE_2D, sprite.texture.id);
				glUniformMatrix4fv(Sprite::_trns, 1, GL_FALSE, &sprite.trns[0][0]);
				glUniform4fv(Sprite::_location, 1, &sprite.location[0]);
				glDrawArrays(GL_QUADS, 0, (GLsizei)vtxs.size());
			});

			glBindTexture(GL_TEXTURE_2D, 0);
			glDisableVertexAttribArray(VEC4_0_16);
			glUseProgram(0);
		};
	};
	inline SpriteManager sprite_manager;
};


namespace loom
{
	inline Sprite::Sprite(Texture& texture, vec2 start, vec2 size, vec2 stride, uint16_t ups) :
		texture(texture),
		location(start, size),
		stride(stride),
		ups(ups),
		mvp(Camera::mvp)
	{
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
};

