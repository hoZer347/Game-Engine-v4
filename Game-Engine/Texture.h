#pragma once

#include "Data.h"

#include <string>

namespace loom
{
	// Texture Object
	struct Texture final :
		public GameObject<Texture>
	{
		uint32_t id = 0;
		int32_t w = -1, h = -1;


	protected:
		friend struct ptr<Texture>;
		Texture(std::string file, uint32_t type);

	private:
		std::string file = "";
		uint32_t type = 0;
		int32_t nrChannels = -1;
	};
};
