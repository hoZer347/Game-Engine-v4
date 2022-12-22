#include "Rendering.h"

#include "Data.h"
#include "Buffer.h"

#include "Shader.hpp"
#include "Texture.hpp"

#include <GLEW/glew.h>
#include <GLFW/glfw3.h>

namespace loom
{
	std::unordered_map<GLFWwindow*, ShaderManager*> window_to_shader;
	std::unordered_map<GLFWwindow*, TextureManager*> window_to_texture;

	struct Deallocator
	{
		virtual ~Deallocator()
		{
			for (auto& i : window_to_shader) delete i.second;
			for (auto& i : window_to_texture) delete i.second;
		};
	};

	static inline Deallocator deallocator;


	void create_shader(Shader& dst, std::string file_name)
	{
		if (GLFWwindow* window = glfwGetCurrentContext())
		{
			if (!window_to_shader[window])
				window_to_shader[window] = new ShaderManager();
			dst = window_to_shader[window]->create(file_name);
		}
		else
		{
			std::cerr << "No Window Present" << std::endl;
		};
	};
	void create_shader(Shader& dst, std::vector<std::string> file_names)
	{
		if (GLFWwindow* window = glfwGetCurrentContext())
		{
			if (!window_to_shader[window])
				window_to_shader[window] = new ShaderManager();
			dst = window_to_shader[window]->create(file_names);
		}
		else
		{
			std::cerr << "No Window Present" << std::endl;
			exit(0);
		};
	};
	void create_texture(Texture& dst, std::string file_name, Texture_Type type)
	{
		if (GLFWwindow* window = glfwGetCurrentContext())
		{
			if (!window_to_texture[window])
				window_to_texture[window] = new TextureManager();
			dst = window_to_texture[window]->create(file_name, type);
		}
		else
		{
			std::cerr << "No Window Present" << std::endl;
			exit(0);
		};
	};


	Task _create_shader(Shader& dst, std::string file_name) noexcept
	{
		return [&dst, file_name]()
		{
			if (GLFWwindow* window = glfwGetCurrentContext())
			{
				if (!window_to_shader[window])
					window_to_shader[window] = new ShaderManager();
				dst = window_to_shader[window]->create(file_name);
			}
			else
			{
				std::cerr << "No Window Present" << std::endl;
				exit(0);
			};
		};
	};
	Task _create_shader(Shader& dst, std::vector<std::string> file_names) noexcept
	{
		return [&dst, file_names]()
		{
			if (GLFWwindow* window = glfwGetCurrentContext())
			{
				if (!window_to_shader[window])
					window_to_shader[window] = new ShaderManager();
				dst = window_to_shader[window]->create(file_names);
			}
			else
			{
				std::cerr << "No Window Present" << std::endl;
				exit(0);
			};
		};
	};
	Task _create_texture(Texture& dst, std::string file_name, Texture_Type type) noexcept
	{
		return [&dst, file_name, type]()
		{
			if (GLFWwindow* window = glfwGetCurrentContext())
			{
				if (!window_to_texture[window])
					window_to_texture[window] = new TextureManager();
				dst = window_to_texture[window]->create(file_name, type);
			}
			else
			{
				std::cerr << "No Window Present" << std::endl;
				exit(0);
			};
		};
	};


	Task _render(Buffer<Mesh>& buffer) noexcept
	{
		// TODO: incorporate other GL_TEXTURE types

		return buffer._view([](const Mesh& mesh)
		{
			glUseProgram((GLuint)mesh.shader);
			
			glBindTexture(GL_TEXTURE_2D, (GLuint)mesh.texture);

			glBufferData(
				GL_ARRAY_BUFFER,
				mesh.vtxs.size() * sizeof(Vtx),
				mesh.vtxs.data(),
				GL_DYNAMIC_DRAW);

			glBufferData(
				GL_ELEMENT_ARRAY_BUFFER,
				mesh.inds.size() * sizeof(Ind),
				mesh.inds.data(),
				GL_STATIC_DRAW);

			glDrawElements((GLenum)mesh.draw_mode, (GLsizei)mesh.inds.size(), GL_UNSIGNED_INT, nullptr);

			glBindTexture(GL_TEXTURE_2D, 0);
		});
	};
};
