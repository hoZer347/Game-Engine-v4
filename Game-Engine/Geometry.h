#pragma once

#include "Mesh.h"
#include "Loom.h"
#include "Enums.h"
#include "Camera.h"
#include "Shader.h"

#include "GLEW/glew.h"
#include "GLFW/glfw3.h"

#include <glm/glm.hpp>
using namespace glm;

namespace loom
{
	namespace geo
	{
		inline vec4* color;
		inline mat4* trns;

		inline Shader shader { "Geometry", "mvp", &Camera::mvp, "trns", trns, "color", color };

		template <auto collision_type, auto vtx_size, auto render_type, auto num_vtxs, auto num_inds>
		struct Geometry : public Mesh<vtx_size, render_type, num_vtxs, num_inds>
		{
			Geometry() : Mesh<vtx_size, render_type, num_vtxs, num_inds>(shader)
			{ };

			mat4 trns{ 1 };
			mat4* mvp{ &Camera::mvp };
			vec4 color{ 1 };

		private:
			inline void subRender() override
			{
				geo::color = &this->color;
				geo::trns = &this->trns;
			};
		};
		

		template <auto vtx_size>
		struct Cube final :
			public Geometry<COLLISION::CUBE, vtx_size, GL_QUADS, 8, 24>
		{
			Cube()
			{
				this->modify(0,	vec4(0, 0, 0, 1),	// 0
							vec4(1, 0, 0, 1),	// 1
							vec4(1, 1, 0, 1),	// 2
							vec4(0, 1, 0, 1),	// 3
							vec4(0, 0, 1, 1),	// 4
							vec4(1, 0, 1, 1),	// 5
							vec4(1, 1, 1, 1),	// 6
							vec4(0, 1, 1, 1));	// 7
				this->index(0,	vec4(3, 2, 1, 0),	// Front
							vec4(4, 5, 6, 7),	// Back
							vec4(3, 0, 4, 7),	// Left
							vec4(1, 2, 6, 5),	// Right
							vec4(0, 1, 5, 4),	// Bottom
							vec4(2, 3, 7, 6));	// Top
			};
		};


		template <auto vtx_size>
		struct Rect final : public Geometry<COLLISION::CUBE, vtx_size, GL_QUADS, 8, 24>
		{
			Rect()
			{
				this->modify(0, vec4(0, 0, 0, 1),
					vec4(1, 0, 0, 1),
					vec4(1, 1, 0, 1),
					vec4(0, 1, 0, 1));
				this->index(0, 0, 1, 2, 3);
			};
		};
	};
};
