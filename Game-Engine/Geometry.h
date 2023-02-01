#pragma once

#include "Data.h"

#include <glm/glm.hpp>
using namespace glm;

#include <vector>

namespace loom
{
	typedef uint32_t ID;
	typedef uint32_t TYPE;

	struct Shader;
	struct Texture;

	// Mesh of squares
	struct Squares final
	{
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
		
		void load();
		void render();
		void unload();

		static inline ID _buffer = 0;
	};
	//
};
