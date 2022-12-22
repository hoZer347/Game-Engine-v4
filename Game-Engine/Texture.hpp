#pragma once

#include "GLEW/glew.h"
#include "GLFW/glfw3.h"

#include "Data.h"

#include <unordered_map>
#include <string>

#include <iostream>

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

namespace loom {
	struct TextureManager {
		std::unordered_map<std::string, Texture> TEXS;

		Texture create(std::string file_name, Texture type)
		{
			// Catching Duplicates
			std::string s = std::string(file_name);
			if (TEXS[s])
				return TEXS[s];
			//

			// Creating Image Data
			int height, width, nrChannels;
			unsigned char* data = stbi_load(file_name.c_str(), &width, &height, &nrChannels, 0);
			//

			// Creating OPENGL Texture
			unsigned int texture;
			glGenTextures(1, &texture);
			glBindTexture(GL_TEXTURE_2D, texture);
			glTexImage2D(GL_TEXTURE_2D, 0, (GLint)type, width, height, 0, (GLint)type, GL_UNSIGNED_BYTE, data);
			glGenerateMipmap(GL_TEXTURE_2D);
			//

			// Deallocation
			glBindTexture(GL_TEXTURE_2D, 0);
			stbi_image_free(data);
			//

			TEXS[s] = texture;

			return texture;
		};
	};
};
