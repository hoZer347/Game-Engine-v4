#pragma once

#include "Task.h"
#include "System.h"

#include "glm/glm.hpp"
using namespace glm;

namespace loom
{
#ifndef VTX_IMPL
	struct alignas(64) Vtx
	{
		vec4 data[4] = { vec4(0), vec4(0), vec4(0), vec4(0), };
		mat4 mat;
		float floats[16];

		~Vtx() { };
	};
#else
	VTX_IMPL
#endif

#ifndef DATA_IMPL
	typedef uint32_t Ind;

	struct alignas(64) Data
	{
		std::vector<Vtx> vtxs;
		std::vector<Ind> inds;
	};
#else
	DATA_IMPL
#endif

	struct Window;

	typedef uint32_t Shader;
	typedef uint32_t Texture;
	typedef uint32_t DrawMode;

	struct Rendarable
	{
		
	};
};
