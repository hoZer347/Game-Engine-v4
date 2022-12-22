#include "Data.h"

#include "Buffer.h"
#include "Rendering.h"

// OpenGL Includes
#include <GLEW/glew.h>
#include <GLFW/glfw3.h>
//

namespace loom
{
	namespace geo
	{
		_NODISCARD Task _create_square	(Buffer<Mesh>& buffer)
		{
			return buffer._allocate(1, [](Mesh& mesh)
			{
				if (GLFWwindow* window = glfwGetCurrentContext())
				{
					mesh =
					{
						{
							{ vec4(0, 0, 0, 1), vec4(1, 1, 1, 1), vec4(1, 1, 1, 1), vec4(0, 0, 0, 1), },
							{ vec4(1, 0, 0, 1), vec4(1, 1, 1, 1), vec4(1, 1, 1, 1), vec4(1, 0, 0, 1), },
							{ vec4(1, 1, 0, 1), vec4(1, 1, 1, 1), vec4(1, 1, 1, 1), vec4(1, 1, 0, 1), },
							{ vec4(0, 1, 0, 1), vec4(1, 1, 1, 1), vec4(1, 1, 1, 1), vec4(0, 1, 0, 1), },
						}, { 0, 1, 2, 3 },
					};

					mesh.draw_mode = GL_QUADS;

					create_shader(mesh.shader, "shaders/default");
					create_texture(mesh.texture, "Resources/stone.png", GL_RGB);
				};
			});
		};

		_NODISCARD Task _create_cube	(Buffer<Mesh>& buffer)
		{
			return [&buffer]()
			{
				if (GLFWwindow* window = glfwGetCurrentContext())
				{

				};
			};
		};
	};
};
