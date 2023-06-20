#include "Texture.h"

#include "Engine.h"

#include "GLEW/glew.h"
#include "GLFW/glfw3.h"

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

#include <unordered_map>
#include <string>

namespace loom
{
	static inline std::unordered_map<std::string, uint32_t> TEXS;
	Texture::Texture(std::string file, uint32_t type)
	{
		Engine::DoOnMain([=]()
		{
			// Catching Duplicates
			if (TEXS[file])
				id = TEXS[file];
			//

			// Creating Image Data
			stbi_set_flip_vertically_on_load(true);
			unsigned char* data = stbi_load(file.c_str(), &w, &h, &nrChannels, 0);
			//

			// Creating OPENGL Texture
			glGenTextures(1, &id);
			glBindTexture(GL_TEXTURE_2D, id);
			glTexImage2D(GL_TEXTURE_2D, 0, type, w, h, 0, type, GL_UNSIGNED_BYTE, data);
			glGenerateMipmap(GL_TEXTURE_2D);
			//

			// Deallocation
			glBindTexture(GL_TEXTURE_2D, 0);
			stbi_image_free(data);
			//

			glGetError();
		});
	};
};