#pragma once

#include "Data.h"

namespace loom
{
	struct Squares final : public Object, public Renderable
	{
		Squares() : Object()
		{ };

		Squares(Shader& shader)
		: shader(shader), Object()
		{ };

		Squares(Texture& texture)
		: texture(texture), Object()
		{ };

		Squares(Shader& shader, Texture& texture)
		: shader(shader), texture(texture), Object()
		{ };

		mat4 trns = mat4(1);
		std::vector<float> data;

	private:
		void load() override;
		void render() override;
		void unload() override;

		
		Shader& shader = _shader;
		Texture& texture = _texture;

		static inline ID _buffer = 0;
		static inline Shader _shader{ "shaders/Squares" };
		static inline Texture _texture{ "Resources/stone.png", 0x1907 };
	};
};
