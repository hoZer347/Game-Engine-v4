#include "Sprite.h"

#include "Enums.h"
#include "Camera.h"

#include <GLEW/glew.h>
#include <GLFW/glfw3.h>

#include <glm/gtx/transform.hpp>

#include <vector>

namespace loom
{
	static inline uint32_t _mvp;
	static inline uint32_t _trns;
	static inline uint32_t _location;
	static inline Shader shader{ "Sprite" };


	Sprite::Sprite(Texture& texture, vec2 start, vec2 size, vec2 stride, uint16_t ups) :
		texture(texture),
		location(start, size),
		stride(stride),
		ups(ups)
	{ mvp.bind(Camera::mvp); };
	void Sprite::load()
	{
		location.x /= texture.w;
		location.y /= texture.h;
		location.z /= texture.w;
		location.w /= texture.h;

		glBindTexture(GL_TEXTURE_2D, texture.id);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glBindTexture(GL_TEXTURE_2D, 0);

		if (!_mvp)
			_mvp = glGetUniformLocation(shader.id, "mvp");
		if (!_trns)
			_trns = glGetUniformLocation(shader.id, "trns");
		if (!_location)
			_location = glGetUniformLocation(shader.id, "loc");
	};
	void Sprite::update()
	{
		// Update the sprite clock here


		static int i = 0;
		i++;
	};
	void Sprite::render()
	{
		glUseProgram(shader.id);
		glEnableVertexAttribArray(VEC4_0_16);

		glBufferData(GL_ARRAY_BUFFER, sizeof(vec4) * vtxs.size(), vtxs.data(), GL_STATIC_DRAW);
		
		glUniformMatrix4fv(_trns, 1, GL_FALSE, &trns[0][0]);
		glUniformMatrix4fv(_mvp, 1, GL_FALSE, &mvp[0][0]);
		glUniform4fv(_location, 1, &location[0]);
		glBindTexture(GL_TEXTURE_2D, texture.id);
		glDrawArrays(GL_QUADS, 0, (GLsizei)Sprite::vtxs.size());

		glBindTexture(GL_TEXTURE_2D, 0);
		glDisableVertexAttribArray(VEC4_0_16);
		glUseProgram(0);
	};
};
