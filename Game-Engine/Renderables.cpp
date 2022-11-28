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

	void Renderable::load(std::shared_ptr<Window> window)
	{
		Renderable::window = window;

		window->assign([]()
			{
				Buffer<Mesh>::view([](const auto& i)
				{
					i->render();
				});

				return should_draw.load();
			});
	};

	void Mesh::render()
	{
		glUseProgram(shader);

		for (auto& tex : texs)
			glBindTexture(GL_TEXTURE_2D, tex);

		glBufferData(
			GL_ARRAY_BUFFER,
			vtxs.size() * sizeof(Vtx),
			vtxs.data(),
			GL_DYNAMIC_DRAW);

		glBufferData(
			GL_ELEMENT_ARRAY_BUFFER,
			inds.size() * sizeof(unsigned int),
			inds.data(),
			GL_STATIC_DRAW);

		glDrawElements(draw_mode, (GLsizei)inds.size(), GL_UNSIGNED_INT, nullptr);

		for (auto& tex : texs)
			glBindTexture(GL_TEXTURE_2D, 0);
	};

	void Mesh::test()
	{
		auto w0 = Data<Window>::create();

		Buffer<Mesh>::create(1);
		Buffer<Mesh>::modify([w0](auto vec)
			{
				vec[0]->vtxs =
				{
					{
						{ vec4(-1, -1, 0, 1), vec4(1), vec4(1), vec4(0, 0, 0, 0), },
						{ vec4( 1, -1, 0, 1), vec4(1), vec4(1), vec4(1, 0, 0, 0), },
						{ vec4( 1,  1, 0, 1), vec4(1), vec4(1), vec4(1, 1, 0, 0), },
						{ vec4(-1,  1, 0, 1), vec4(1), vec4(1), vec4(0, 1, 0, 0), },
					}
				};
				vec[0]->inds = { 0, 1, 2, 3 };

				vec[0]->load(w0);
				vec[0]->load_shader({ "Sprite" });
				vec[0]->load_textures({ { "Resources/DK.png", GL_RGBA } });
			});
		Buffer<Mesh>::swap();

		while (!w0->get_kill());
	};
};
