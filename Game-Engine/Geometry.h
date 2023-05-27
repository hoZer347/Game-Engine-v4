#pragma once

#include "Data.h"
#include <glm/glm.hpp>
using namespace glm;

#include <vector>

namespace loom
{
	struct Cube final : virtual private Loadable, virtual private Renderable
	{
		Cube()
		{ };

		vec4 color = vec4(0, 0, 1, 1);
		mat4 trns = mat4(1);

	private:
		void load() override;
		void render() override;

		static inline std::vector<vec4> vtxs =
		{
			vec4(0, 0, 0, 1),
			vec4(1, 0, 0, 1),
			vec4(1, 1, 0, 1),
			vec4(0, 1, 0, 1),

			vec4(0, 0, 1, 1),
			vec4(1, 0, 1, 1),
			vec4(1, 1, 1, 1),
			vec4(0, 1, 1, 1),
		};
		static inline std::vector<uint32_t> inds =
		{
			0, 1, 2, 3,
			7, 6, 5, 4,
			1, 5, 6, 2,
			4, 0, 3, 7,
			0, 1, 5, 4,
			3, 2, 6, 7,
		};
	};


	struct Plane final : virtual private Loadable, virtual private Renderable
	{
		vec4 color = vec4(1);
		mat4 trns = mat4(1);

	private:
		void load() override;
		void render() override;

		static inline std::vector<vec4> vtxs =
		{
			vec4(0, 0, 0, 1),
			vec4(1, 0, 0, 1),
			vec4(1, 1, 0, 1),
			vec4(0, 1, 0, 1),
		};
	};


	struct Circle final :
		virtual private Loadable,
		virtual private Renderable
	{
		vec4 color = vec4(1);
		mat4 trns;

	private:
		void load() override;
		void render() override;
	};
};
