#pragma once

#include "Data.h"
#include "Shader.h"
#include "Texture.h"
#include "Camera.h"

namespace loom
{
	struct Sprite;

	struct SpriteManager :
		virtual public Updateable,
		virtual public Renderable
	{
		void update() override;
		void render() override;

		static inline std::recursive_mutex mut;
		static inline Shader shader{ "Sprite" };
		static inline std::vector<vec4> vtxs
		{
			{ 0, 0, 0, 1 },
			{ 1, 0, 0, 1 },
			{ 1, 0, 1, 1 },
			{ 0, 0, 1, 1 },
		};

		static inline ptr<SpriteManager> manager;
	};


	struct Sprite :
		public GameObject<Sprite>
	{
		Sprite(Texture& texture, vec2 start, vec2 size, vec2 stride, uint16 ups) :
			texture(texture),
			location(start, size),
			stride(stride),
			ups(ups)
		{
			Engine::DoOnMain([this, &texture]()
			{
				if (!this) return;

				location.x /= texture.w;
				location.y /= texture.h;
				location.z /= texture.w;
				location.w /= texture.h;

				glBindTexture(GL_TEXTURE_2D, texture.id);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
				glBindTexture(GL_TEXTURE_2D, 0);
			});
		};

		Texture& texture;

		vec2 stride;
		uint16 ups;
		mat4 trns{ 1 };
		vec4 location;
	};


	void SpriteManager::update()
	{
		GameObject<Sprite>::access([](Sprite& sprite)
		{
			
		});
	};
	void SpriteManager::render()
	{
		glUseProgram(shader.id);

		glEnableVertexAttribArray(VEC4_0_16);

		glUniformMatrix4fv(glGetUniformLocation(shader.id, "mvp"), 1, false, (float*)&Camera::mvp);

		glBufferData(
			GL_ARRAY_BUFFER,
			vtxs.size() * sizeof(vec4),
			vtxs.data(),
			GL_DYNAMIC_DRAW);

		GameObject<Sprite>::access([](Sprite& sprite)
		{
			glBindTexture(GL_TEXTURE_2D, sprite.texture.id);

			glUniformMatrix4fv(glGetUniformLocation(shader.id, "trns"), 1, false, (float*)&sprite.trns);
			glUniform4fv(glGetUniformLocation(shader.id, "loc"), 1, (float*)&sprite.location);

			glDrawArrays(GL_QUADS, 0, 4);

			glBindTexture(GL_TEXTURE_2D, 0);
		});

		glDisableVertexAttribArray(VEC4_0_16);
	};
};
