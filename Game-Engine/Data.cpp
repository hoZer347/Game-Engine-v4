#include "Data.h"

#include "GLEW/glew.h"
#include "GLFW/glfw3.h"

#include "Shader.hpp"
#include "Texture.hpp"

namespace loom
{
	thread_local static ShaderManager* s_mgr = nullptr;
	thread_local static TextureManager* t_mgr = nullptr;

	RenderObj::~RenderObj()
	{
		// TODO: Prevent renderobjs from being destroyed during runtime
	};


	void _Buffer::AddAttribute(size_t size, std::string name)
	{ attributes.push_back({ size, name }); };


	Mesh::~Mesh()
	{
		// Unloading Shaders
		//

		// Unloading Textures
		//

		// Unloading Draw Command
		//


		// Unloading Buffers;
		//for (auto& buffer : buffers)
		//	glDeleteBuffers(1, &buffer->id);
		//
	};

	void Mesh::load()
	{
		if (GLFWwindow* window = glfwGetCurrentContext())
		{
			// Loading Shader
			if (!s_mgr)
				s_mgr = new ShaderManager();

			shader.id = s_mgr->create(shader.files);
			glUseProgram(shader.id);
			//


			// Loading Textures
			if (!t_mgr)
				t_mgr = new TextureManager();

			for (auto& file : textures.files)
				textures.ids.push_back(t_mgr->create(file, GL_RGB));
			//


			// Loading Draw Command
			//


			// Loading Buffers
			for (auto& buffer : buffers)
			{
				//glGenBuffers(1, &buffer->id);
				//glBindBuffer(GL_ARRAY_BUFFER, buffer->id);
			};
			//
		};
	};
};
