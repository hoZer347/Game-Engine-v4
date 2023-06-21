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
		inline vec4* _color;
		inline mat4* _trns;
		inline mat4* _mvp = &Camera::mvp;

		inline Shader shader { "Geometry", "mvp", &_mvp, "trns", &_trns, "color", &_color };

		template <auto collision_type, auto vtx_size, auto render_type, auto num_vtxs, auto num_inds>
		struct Geometry : public Mesh<vtx_size, render_type, num_vtxs, num_inds>
		{
			Geometry(Shader& shader = geo::shader) :
				Mesh<vtx_size, render_type, num_vtxs, num_inds>(shader)
			{ };

			mat4 trns{ 1 };
			mat4* mvp{ &Camera::mvp };
			vec4 color{ 0, 0, 1, 1 };

		private:
			inline virtual void subRender() override
 			{
				geo::_color = &this->color;
				geo::_trns = &this->trns;
			};
		};
		

		template <auto vtx_size = 4, auto render_type = GL_QUADS>
		struct Cube :
			public Geometry<COLLISION::CUBE, vtx_size, render_type, 8, 24>
		{
			Cube(vec4 pos = vec4(0), Shader& shader = geo::shader) :
				Geometry<COLLISION::CUBE, vtx_size, render_type, 8, 24>(shader)
			{
				static_assert(
					render_type == GL_QUADS ||
					render_type == GL_LINES,
					"Drawing Cubes only works with GL_QUADS and GL_LINES for now");

				this->modify(0,
							vec4(0, 0, 0, 1),	// 0
							vec4(1, 0, 0, 1),	// 1
							vec4(1, 1, 0, 1),	// 2
							vec4(0, 1, 0, 1),	// 3
							vec4(0, 0, 1, 1),	// 4
							vec4(1, 0, 1, 1),	// 5
							vec4(1, 1, 1, 1),	// 6
							vec4(0, 1, 1, 1));	// 7
				this->index(0,
							vec4(3, 2, 1, 0),	// Front
							vec4(4, 5, 6, 7),	// Back
							vec4(3, 0, 4, 7),	// Left
							vec4(1, 2, 6, 5),	// Right
							vec4(0, 1, 5, 4),	// Bottom
							vec4(2, 3, 7, 6));	// Top
			};
		};


		template <auto vtx_size = 4, auto render_type = GL_QUADS>
		struct Rect : public Geometry<COLLISION::CUBE, vtx_size, render_type, 4, 4>
		{
			static_assert(
				render_type == GL_QUADS ||
				render_type == GL_LINES,
				"Drawing Rects only works with GL_QUADS and GL_LINES for now");

			Rect(vec4 pos = vec4(0)) :
				Geometry<COLLISION::CUBE, vtx_size, render_type, 4, 4>(geo::shader)
			{
				this->modify(0,
					pos + vec4(0, 0, 0, 1),
					pos + vec4(1, 0, 0, 1),
					pos + vec4(1, 1, 0, 1),
					pos + vec4(0, 1, 0, 1));

				switch (render_type)
				{
				case GL_QUADS:
					this->index(0,
						0, 1, 2, 3);
					break;
				case GL_LINES:
					this->index(0,
						0, 1, 1, 2, 2, 3, 3, 0);
				};
			};
			Rect(Shader& shader) :
				Geometry<COLLISION::CUBE, vtx_size, render_type, 4, 4>(shader)
			{ };

		private:
			inline void subRender() override
			{
				geo::_color = &this->color;
				geo::_trns = &this->trns;
			};
		};
	};
};
