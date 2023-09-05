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
	private:
		void update() override;
		void render() override;

		static inline std::recursive_mutex mut;
		static inline ptr<Shader> shader{ 0, "Sprite" };
		static inline ptr<Shader> grayscale_shader{ 0, "Sprite.vert", "GrayScaleSprite.frag" };
		static inline std::vector<vec4> vtxs
		{
			{ 0, 0, 0, 1 },
			{ 1, 0, 0, 1 },
			{ 1, 0, 1, 1 },
			{ 0, 0, 1, 1 },
		};
		static inline ptr<SpriteManager> manager { 0 };
	};


	struct Sprite final :
		public GameObject<Sprite>
	{
		Sprite(ptr<Texture> texture, vec2 start, vec2 size, vec2 stride, uint16 ups) :
			texture(texture),
			location(start, size),
			stride(stride),
			ups(ups)
		{
			Engine::DoOnMain([this]()
			{
				if (!this) return;

				location.x /= this->texture->w;
				location.y /= this->texture->h;
				location.z /= this->texture->w;
				location.w /= this->texture->h;

				this->stride.x /= this->texture->w;
				this->stride.y /= this->texture->h;

				glBindTexture(GL_TEXTURE_2D, this->texture->id);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
				glBindTexture(GL_TEXTURE_2D, 0);
			});
		};

		bool grayscale = false;

		ptr<Texture> texture;

		vec2 stride;
		uint16 ups;
		mat4 trns{ 1 };
		vec4 location;
	};


	void SpriteManager::update()
	{
		static Timer timer;

		if (timer.GetDiff_mls() < 1000.f / 4)
			return;
		timer.push(std::chrono::milliseconds(1000 / 4));

		static uint64 i = 0;
		
		std::cout << i << std::endl;

		GameObject<Sprite>::access([](Sprite& sprite)
		{
			if (i < 2)	sprite.offset += sprite.stride;
			else		sprite.offset -= sprite.stride * vec2(4, 4);
		});

		if (i > 2) i = 0;
		else i++;
	};
	void SpriteManager::render()
	{
		glEnableVertexAttribArray(VEC4_0_16);

		glBufferData(
			GL_ARRAY_BUFFER,
			vtxs.size() * sizeof(vec4),
			vtxs.data(),
			GL_DYNAMIC_DRAW);

		GameObject<Sprite>::access([](Sprite& sprite)
		{
			if (sprite.grayscale)
				glUseProgram(grayscale_shader->id);
			else
				glUseProgram(shader->id);

			glBindTexture(GL_TEXTURE_2D, sprite.texture->id);

			glUniformMatrix4fv(glGetUniformLocation(shader->id, "mvp"), 1, false, (float*)&Camera::mvp);
			glUniformMatrix4fv(glGetUniformLocation(shader->id, "trns"), 1, false, (float*)&sprite.trns);
			glUniform4fv(glGetUniformLocation(shader->id, "loc"), 1, (float*)&sprite.location);

			glDrawArrays(GL_QUADS, 0, 4);

			glBindTexture(GL_TEXTURE_2D, 0);
		});

		glDisableVertexAttribArray(VEC4_0_16);
	};
};
