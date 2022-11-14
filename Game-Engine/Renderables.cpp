#include "Renderables.h"

#include <GLEW/glew.h>
#include <GLFW/glfw3.h>

#include "Engine.h"

#include "Texture.hpp"
#include "Shader.hpp"

namespace eng
{
	void Renderable::load_shader(std::vector<std::string> file_names)
	{
		window->load_shader(file_names, shader);
	};

	void Renderable::load_textures(std::vector<std::pair<std::string, unsigned int>> file_names)
	{
		window->load_textures(file_names, texs);
	};

	Mesh::Mesh()
	{ };

	void Mesh::load(std::shared_ptr<Window> window)
	{
		this->window = window;

		window->assign([this]()
			{
				glUseProgram(shader);

				for (auto& tex : texs)
					glBindTexture(GL_TEXTURE_2D, tex);

				auto& vec = Buffer<Mesh::r_Obj>::view_vec();

				for (auto i = 0; i < vec.size(); i++)
				{
					glBufferData(
						GL_ARRAY_BUFFER,
						vec[i]->vtxs.size() * sizeof(Vtx),
						vec[i]->vtxs.data(),
						GL_DYNAMIC_DRAW);

					glBufferData(
						GL_ELEMENT_ARRAY_BUFFER,
						vec[i]->inds.size() * sizeof(unsigned int),
						vec[i]->inds.data(),
						GL_STATIC_DRAW);
						glDrawElements(draw_mode, (GLsizei)vec[i]->inds.size(), GL_UNSIGNED_INT, nullptr);
				};

				for (auto& tex : texs)
					glBindTexture(GL_TEXTURE_2D, 0);
			});
	};

	Sprite::Sprite()
	{
		
	};

	void Sprite::load(std::shared_ptr<Window> window)
	{
		this->window = window;


	};
};
