#include "Sprite.h"

#include "Mem_Leak.h"

#include <GLEW/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
using namespace glm;

#include "Attribute.hpp"
#include "Shader.hpp"
#include "Engine.h"

#include <iostream>

namespace eng
{
	DEFINE_TASK(InitSprite, void*, )
	{
		Sprite::shader = ShaderManager::create("Sprite");

		glVertexAttribPointer(
			Sprite::atb1,
			3,
			GL_FLOAT, GL_FALSE,
			5 * sizeof(float),
			(void*)(0 * sizeof(float)));

		glVertexAttribPointer(
			Sprite::atb2,
			2,
			GL_FLOAT, GL_FALSE,
			5 * sizeof(float),
			(void*)(3 * sizeof(float)));

		delete this;
	};
	DEFINE_TASK(RenderSprite, void*, )
	{
		Object<Sprite>::access([](Sprite& spr) {
			glUseProgram(spr.shader);

			glBindTexture(GL_TEXTURE_2D, spr.tex);

			glEnableVertexAttribArray(spr.atb1);
			glEnableVertexAttribArray(spr.atb2);

			glBufferData(
				GL_ARRAY_BUFFER,
				spr.vtxs.size() * sizeof(float),
				spr.vtxs.data(),
				GL_DYNAMIC_DRAW);

			glBufferData(
				GL_ELEMENT_ARRAY_BUFFER,
				spr.inds.size() * sizeof(unsigned int),
				spr.inds.data(),
				GL_DYNAMIC_DRAW);

			glDrawElements(GL_QUADS, (GLsizei)spr.inds.size(), GL_UNSIGNED_INT, 0);

			glBindTexture(GL_TEXTURE_2D, 0);

			glDisableVertexAttribArray(spr.atb1);
			glDisableVertexAttribArray(spr.atb2);
		}, 0, Object<Sprite>::size());
	};

	void Sprite::init(Thread* thread)
	{
		thread->push(InitSprite::create(nullptr).release());
		thread->assign(RenderSprite::create(nullptr).release());
	};
};
