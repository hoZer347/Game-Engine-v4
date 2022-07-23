#pragma once

#include "glm/glm.hpp"
using namespace glm;

#include <vector>

namespace eng
{
	typedef const unsigned int MemoryID;
	typedef const unsigned int ThreadID;

	struct alignas(64) Material
	{
		unsigned int	shader = 0,
						draw_mode = 0x0007;  // GL_QUADS, GL_TRIANGLES, etc.

		std::vector<unsigned int> texs;

		// 24 Free Bytes
	};
		
	struct alignas(64) Vtx
	{
		vec4	pos,
				cdr,
				clr,
				nrm;
	};

	struct alignas(64) Mesh
	{
		std::vector<Vtx>			vtxs;
		std::vector<unsigned int>	inds;
	};
};
