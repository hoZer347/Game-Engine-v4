#include "Texture.h"

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

namespace loom
{
	TextureManager::TextureManager()
	{
		stbi_set_flip_vertically_on_load(true);
		TEXS = { };
	};
	TextureManager::~TextureManager()
	{
		for (auto& i : TEXS)
			glDeleteTextures(1, &i.second);
	};

	GLuint TextureManager::create(std::string file_name, GLuint type)
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
		GLuint texture;
		glGenTextures(1, &texture);
		glBindTexture(GL_TEXTURE_2D, texture);
		glTexImage2D(GL_TEXTURE_2D, 0, type, width, height, 0, type, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);
		//

		// Deallocation
		glBindTexture(GL_TEXTURE_2D, 0);
		stbi_image_free(data);
		//

		return TEXS[s] = texture;
	};
};
