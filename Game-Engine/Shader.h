#pragma once

#include <string>
#include <vector>
#include <unordered_map>

namespace loom
{
	// Shader Object
	struct Shader final
	{
		uint32_t id = 0;

		Shader(std::string&& name);
		Shader(std::vector<std::string>&& files);

	private:
		std::vector<std::string> files;
	};
};
