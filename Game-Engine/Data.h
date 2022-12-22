#pragma once

#include "Task.h"

#include "glm/glm.hpp"
using namespace glm;

#include <vector>

#include <iostream>

namespace loom
{
	template <typename T> struct Buffer;

	struct alignas(64) Vtx final
	{
		vec4 data[4] = { vec4(0), vec4(0), vec4(0), vec4(0), };
	};

	typedef uint32_t _ID;
	typedef _ID Shader;
	typedef _ID Texture;
	typedef _ID Texture_Type;
	typedef _ID DrawMode;
	typedef _ID Ind;
	typedef std::vector<Ind> Inds;
	typedef std::vector<Vtx> Vtxs;

	struct Mesh final
	{
		Vtxs		vtxs;
		Inds		inds;

		mat4		trns = mat4(1);
		DrawMode	draw_mode;
		Shader		shader;
		Texture		texture;
	};

	namespace geo
	{
		_NODISCARD Task _create_square	(Buffer<Mesh>& buffer);
		_NODISCARD Task _create_cube	(Buffer<Mesh>& buffer);
	};
};
