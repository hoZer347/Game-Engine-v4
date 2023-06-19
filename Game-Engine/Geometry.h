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

	struct Geometry : public Mesh
	{
		Geometry(auto&& collision_type, auto&& num_vtxs, auto&& num_inds, auto&& render_type = 0x0004);

		mat4 trns{ 1 };
		mat4* mvp{ &Camera::mvp };
		vec4 color{ 1 };

		const uint8 collision_type;

		static inline ptr<Shader> shader{ "Geometry" };
		static inline uint32_t _color = 0;
		static inline uint32_t _trns = 0;
		static inline uint32_t _mvp = 0;

	private:
		void subRender() override;
	};


	struct Cube final : public Geometry
	{
		Cube();
		static inline const uint8 collision_type = COLLISION::CUBE;
	};


	struct Rect final : public Geometry
	{
		Rect();
		static inline const uint8 collision_type = COLLISION::SQUARE;
	};


	struct Circle final : public Geometry
	{
		Circle();
		static inline const uint8 collision_type = COLLISION::CIRCLE;

		const float radius = 1.0f;
	};
};
