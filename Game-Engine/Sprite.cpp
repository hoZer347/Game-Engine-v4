#include "Sprite.h"

#include "Enums.h"
#include "Camera.h"

#include <GLEW/glew.h>
#include <GLFW/glfw3.h>

#include <glm/gtx/transform.hpp>

#include <vector>

namespace loom
{
	static inline uint32 _mvp;
	static inline uint32 _trns;
	static inline uint32 _location;
	static inline Shader shader{ "Sprite" };


	struct SpriteManager :
		virtual protected Loadable,
		virtual protected Updateable,
		virtual protected Renderable
	{
		static inline std::atomic<std::vector<Sprite*>*> sprites = new std::vector<Sprite*>();
		static inline std::vector<vec4> vtxs
		{
			vec4(0, 0, 0, 1),
			vec4(1, 0, 0, 1),
			vec4(1, 0, 1, 1),
			vec4(0, 0, 1, 1),
		};

	private:
		void load() override;
		void update() override;
		void render() override;
	};

	SpriteManager manager;


	Sprite::Sprite(Texture& texture, vec2 start, vec2 size, vec2 stride, uint16_t ups) :
		texture(texture),
		vtxs(manager.vtxs),
		location(start, size),
		stride(stride),
		ups(ups),
		mvp(Camera::mvp)
	{
		std::vector<Sprite*>* sprites;
		while (!(sprites = manager.sprites.exchange(nullptr)));

		sprites->push_back(this);
		manager.sprites.exchange(sprites);
	};
	void Sprite::load()
	{
		location.x /= texture.w;
		location.y /= texture.h;
		location.z /= texture.w;
		location.w /= texture.h;

		glBindTexture(GL_TEXTURE_2D, texture.id);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glBindTexture(GL_TEXTURE_2D, 0);
	};
	void SpriteManager::load()
	{
		_mvp = glGetUniformLocation(shader.id, "mvp");
		_trns = glGetUniformLocation(shader.id, "trns");
		_location = glGetUniformLocation(shader.id, "loc");
	};
	void SpriteManager::update()
	{
		// Update the sprite clock here


		static int i = 0;
		i++;
	};
	void SpriteManager::render()
	{
		glUseProgram(shader.id);
		glEnableVertexAttribArray(VEC4_0_16);

		glBufferData(GL_ARRAY_BUFFER, sizeof(vec4) * vtxs.size(), vtxs.data(), GL_STATIC_DRAW);
		
		std::vector<Sprite*>* sprites;
		while (!(sprites = this->sprites.exchange(nullptr)));

		for (auto& sprite : *sprites)
		{
			glBindTexture(GL_TEXTURE_2D, sprite->texture.id);
			glUniformMatrix4fv(_trns, 1, GL_FALSE, &sprite->trns[0][0]);
			glUniformMatrix4fv(_mvp, 1, GL_FALSE, &Camera::mvp[0][0]);
			glUniform4fv(_location, 1, &sprite->location[0]);
			glDrawArrays(GL_QUADS, 0, (GLsizei)SpriteManager::vtxs.size());
		};

		this->sprites.exchange(sprites);

		glBindTexture(GL_TEXTURE_2D, 0);
		glDisableVertexAttribArray(VEC4_0_16);
		glUseProgram(0);
	};
};
