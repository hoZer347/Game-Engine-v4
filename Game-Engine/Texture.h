#pragma once

#include <string>

namespace loom
{
	// Texture Object
	struct Texture final
	{
		uint32_t id = 0;
		int32_t w = -1, h = -1;

		Texture(std::string file, uint32_t type);

	private:
		std::string file;
		uint32_t type = 0;
		int32_t nrChannels = -1;
	};
};
