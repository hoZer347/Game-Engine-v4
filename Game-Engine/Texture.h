#pragma once

#include "GLEW/glew.h"
#include "GLFW/glfw3.h"

#include <unordered_map>
#include <string>

namespace loom {
	struct TextureManager {
		std::unordered_map<std::string, GLuint> TEXS;

		TextureManager();
		virtual ~TextureManager();

		GLuint create(std::string file_name, GLuint type);
	};

	thread_local static inline TextureManager* t_mgr; // Change to static non-pointer
};
