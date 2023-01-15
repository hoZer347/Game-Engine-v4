#pragma once

#include "Data.h"

namespace loom
{
	// Mesh of squares
	struct Squares final : public Object, public Renderable
	{
		Squares()
		{ };

		Squares(Shader& shader)
		: shader(shader)
		{ };

		Squares(Texture& texture)
		: texture(texture)
		{ };

		Squares(Shader& shader, Texture& texture)
		: shader(shader), texture(texture)
		{ };

		void operator*=(mat4 m) { trns *= m; };
		void operator+=(vec4 v)
		{
			data.reserve(32 * sizeof(float));

			data.emplace_back(0.f + v.x);
			data.emplace_back(0.f + v.y);
			data.emplace_back(v.z);
			data.emplace_back(v.w);

			data.emplace_back(0.f);
			data.emplace_back(0.f);
			data.emplace_back(0.f);
			data.emplace_back(0.f);


			data.emplace_back(1.f + v.x);
			data.emplace_back(0.f + v.y);
			data.emplace_back(v.z);
			data.emplace_back(v.w);

			data.emplace_back(1.f);
			data.emplace_back(0.f);
			data.emplace_back(0.f);
			data.emplace_back(0.f);


			data.emplace_back(1.f + v.x);
			data.emplace_back(1.f + v.y);
			data.emplace_back(v.z);
			data.emplace_back(v.w);

			data.emplace_back(1.f);
			data.emplace_back(1.f);
			data.emplace_back(0.f);
			data.emplace_back(1.f);

			
			data.emplace_back(0.f + v.x);
			data.emplace_back(1.f + v.y);
			data.emplace_back(v.z);
			data.emplace_back(v.w);

			data.emplace_back(0.f);
			data.emplace_back(1.f);
			data.emplace_back(0.f);
			data.emplace_back(0.f);
		};

	private:
		mat4 trns = mat4(1);
		std::vector<float> data;
		
		void load() override;
		void render() override;
		void unload() override;

		
		Shader& shader = _shader;
		Texture& texture = _texture;

		static inline ID _buffer = 0;
		static inline Shader _shader{ "shaders/Squares" };
		static inline Texture _texture{ "Resources/stone.png", 0x1907 };
	};
	//
};
