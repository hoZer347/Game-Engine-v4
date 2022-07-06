#include "Sprite.h"

#include "Mem_Leak.h"

#include <GLEW/glew.h>
#include <GLFW/glfw3.h>

#include "Attribute.hpp"
#include "Shader.hpp"

#include <iostream>

namespace eng
{
	namespace spr
	{
		DEFINE_TASK(CreateSprite, SpriteData*, )
		{
			data->atb1 = AttributeManager::create();
			data->atb2 = AttributeManager::create();

			data->shader = ShaderManager::create("Sprite");

			glVertexAttribPointer(
				data->atb1,
				3,
				GL_FLOAT, GL_FALSE,
				5 * sizeof(float),
				(void*)(0 * sizeof(float)));

			glVertexAttribPointer(
				data->atb2,
				2,
				GL_FLOAT, GL_FALSE,
				5 * sizeof(float),
				(void*)(3 * sizeof(float)));

			delete this;
		};

		DEFINE_TASK(RenderSprite, SpriteData*, )
		{
			glUseProgram(data->shader);

			glBindTexture(GL_TEXTURE_2D, data->tex);

			glEnableVertexAttribArray(data->atb1);
			glEnableVertexAttribArray(data->atb2);

			glBufferData(
				GL_ARRAY_BUFFER,
				data->vtxs.size() * sizeof(float),
				data->vtxs.data(),
				GL_DYNAMIC_DRAW);

			glBufferData(
				GL_ELEMENT_ARRAY_BUFFER,
				data->inds.size() * sizeof(unsigned int),
				data->inds.data(),
				GL_DYNAMIC_DRAW);

			glDrawElements(GL_QUADS, (GLsizei)data->inds.size(), GL_UNSIGNED_INT, 0);

			glBindTexture(GL_TEXTURE_2D, 0);

			glDisableVertexAttribArray(data->atb1);
			glDisableVertexAttribArray(data->atb2);
		};

		SpriteData* create(Thread* thread)
		{
			SpriteData* spr = new SpriteData { };

			thread->push(CreateSprite::create(spr).release());
			thread->assign(RenderSprite::create(spr).release());

			return spr;
		};
	};
};
