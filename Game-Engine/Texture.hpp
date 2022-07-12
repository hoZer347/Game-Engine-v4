#include "GLEW/glew.h"
#include "GLFW/glfw3.h"

#include <unordered_map>
#include <string>

#include <iostream>

#include "stb_image.h"

namespace eng {
	class TextureManager {
		static inline std::unordered_map<std::string, unsigned int> TEXS;
	
	public:
		static unsigned int create(const char* file_name, unsigned int type) {

			// Catching Duplicates
			std::string s = std::string(file_name);
			if (TEXS[s])
				return TEXS[s];
			//

			// Creating Image Data
			int height, width, nrChannels;
			unsigned char* data = stbi_load(file_name, &width, &height, &nrChannels, 0);
			//

			// Creating OPENGL Texture
			unsigned int texture; // IMPLEMENT HERE
			glGenTextures(1, &texture);
			glBindTexture(GL_TEXTURE_2D, texture);
			glTexImage2D(GL_TEXTURE_2D, 0, type, width, height, 0, type, GL_UNSIGNED_BYTE, data);
			glGenerateMipmap(GL_TEXTURE_2D);
			//

			// Deallocation
			stbi_image_free(data);
			glBindTexture(GL_TEXTURE_2D, 0);
			//

			return TEXS[s] = texture;
		};
	};
};