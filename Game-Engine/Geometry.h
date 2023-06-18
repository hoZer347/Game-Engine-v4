#pragma once

#include "Data.h"
#include "Mesh.h"
#include "Enums.h"
#include "Camera.h"

#include <glm/glm.hpp>
using namespace glm;

#include <vector>

#ifndef VTX_DEF
#define VTX_DEF vec4
#endif

namespace loom
{
	typedef vec4 Vtx;

	struct Geometry
	{
	protected:
		static inline Shader geo_shader{"Geometry"};
		static inline uint32_t _color = 0;
		static inline uint32_t _trns = 0;
		static inline uint32_t _mvp = 0;

		Shader& shader = geo_shader;
	};


	struct Cube final :
		public Geometry,
		virtual private Renderable
	{
		vec4 color = vec4(0, 0, 1, 1);
		mat4 trns;

		static inline const int collision_type = COLLISION::CUBE;

	private:
		void render() override;

	protected:
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


	struct Rect final :
		public Geometry,
		virtual private Renderable
	{
		vec4 color{ 1 };
		mat4 trns{ 1 };
		mat4& mvp = Camera::mvp;

		static inline const int collision_type = COLLISION::SQUARE;

		std::vector<vec4> vtxs =
		{
			vec4(0, 0, 0, 1),
			vec4(1, 0, 0, 1),
			vec4(1, 1, 0, 1),
			vec4(0, 1, 0, 1),
		};

	private:
		void render() override;
	};


	struct Circle final :
		public Geometry,
		virtual private Renderable
	{
		const float radius = 1.0f;

		vec4 color = vec4(1);
		mat4 trns;

		static inline const int collision_type = COLLISION::CIRCLE;

	private:
		void render() override;

	protected:
		std::vector<vec4> vtxs;
	};
};
