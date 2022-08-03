#pragma once

#include <vector>

#include <string>

#include "glm/glm.hpp"
using namespace glm;

namespace eng
{
	struct alignas(64) Vtx
	{
		vec4	pos,
				clr,
				nrm,
				cdr;
	};

	struct alignas(64) Mesh
	{
		std::vector<Vtx>			vtxs;
		std::vector<unsigned int>	inds;
	};
	
	struct alignas(64) Matl
	{
		std::vector<std::string> shader_files;
		std::vector<std::pair<std::string, unsigned int>> texture_files;
	};
};
