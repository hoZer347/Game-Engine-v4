#pragma once

#include "glm/glm.hpp"
using namespace glm;

#include <vector>

#include <iostream>

namespace loom
{
#ifndef VTX_IMPL
	struct alignas(64) Vtx final
	{
		vec4 data[4] = { vec4(0), vec4(0), vec4(0), vec4(0), };
	};
#else
	VTX_IMPL;
#endif

#ifndef MESH_IMPL
	typedef uint32_t Ind;

	struct alignas(64) Mesh final
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

	struct Renderable final
	{
		Shader shader = 0;
		DrawMode draw_mode = 0x0007;
		
		std::vector<Mesh>		meshes;
		std::vector<Texture>	textures;
	};
};
