#pragma once

#include <vector>

#include "Shader.h"

struct Mesh
{
	std::vector<float> vtxs;
	std::vector<size_t> inds;

	std::vector<unsigned int> texs;
	std::vector<unsigned int> unfs;
	
	unsigned int shader = 0;
	std::vector<unsigned int> atbs;
};
