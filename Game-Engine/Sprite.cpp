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
		virtual public Loadable,
		virtual public Updateable,
		virtual public Renderable
	{
		SpriteManager() { Engine::Add(this); };

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
	static inline SpriteManager manager;


	Sprite::Sprite(Texture& texture, vec2 start, vec2 size, vec2 stride, uint16_t ups) :
		texture(texture),
		location(start, size),
		stride(stride),
		ups(ups),
		mvp(Camera::mvp)
	{ };
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
		GameObject<Sprite>::access([](auto& sprite)
		{

		});
	};
	void SpriteManager::render()
	{
		glUseProgram(shader.id);
		glEnableVertexAttribArray(VEC4_0_16);
		glUniformMatrix4fv(_mvp, 1, GL_FALSE, &Camera::mvp[0][0]);
		glBufferData(GL_ARRAY_BUFFER, vtxs.size() * sizeof(vec4), vtxs.data(), GL_STATIC_DRAW);
		
		GameObject<Sprite>::access([](auto& sprite)
		{
			glBindTexture(GL_TEXTURE_2D, sprite.texture.id);
			glUniformMatrix4fv(_trns, 1, GL_FALSE, &sprite.trns[0][0]);
			glUniform4fv(_location, 1, &sprite.location[0]);
			glDrawArrays(GL_QUADS, 0, (GLsizei)vtxs.size());
		});

		glBindTexture(GL_TEXTURE_2D, 0);
		glDisableVertexAttribArray(VEC4_0_16);
		glUseProgram(0);
	};
};
