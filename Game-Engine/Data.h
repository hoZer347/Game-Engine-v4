#pragma once

#include "glm/glm.hpp"
using namespace glm;

#include <vector>

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
	VTX_IMPL;
#endif

#ifndef MESH_IMPL
	typedef uint32_t Ind;

	struct alignas(64) Mesh
	{
		std::vector<Vtx> vtxs;
		std::vector<Ind> inds;
	};
#else
	MESH_IMPL;
#endif

	typedef uint32_t Shader;
	typedef uint32_t Texture;
	typedef uint32_t DrawMode;

	struct Renderable
	{
		
	};
};
