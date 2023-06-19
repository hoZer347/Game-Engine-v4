#include "Mesh.h"

#include "Data.h"
#include "Enums.h"

#include "GLEW/glew.h"
#include "GLFW/glfw3.h"

#include <iostream>

namespace loom
{
	Mesh::~Mesh()
	{ };


	void Mesh::print()
	{
		std::cout << "Vertices: " << std::endl;
		for (uint32 i = 0; i < num_vtxs * 4; i += vtx_size)
		{
			for (uint32 j = 0; j < vtx_size; j++)
				std::cout << vtxs[i + j] << " ";
			std::cout << std::endl;
		};


		std::cout << "Indices: " << std::endl;
		for (uint32 i = 0; i < num_inds; i++)
			std::cout << inds[i] << " ";
		std::cout << std::endl;


		std::cout << "Vertices in Order: " << std::endl;
		for (uint32 i = 0; i < num_inds; i++)
		{
			for (uint32 j = 0; j < vtx_size; j++)
				std::cout << vtxs[inds[i] + j] << " ";
			std::cout << std::endl;
		};
	};
	void Mesh::render()
	{
		glUseProgram(shader->id);

		switch (vtx_size)
		{
		case 16:
			glEnableVertexAttribArray(VEC4_0_64);
			glEnableVertexAttribArray(VEC4_1_64);
			glEnableVertexAttribArray(VEC4_2_64);
			glEnableVertexAttribArray(VEC4_3_64);
			break;

		case 8:
			glEnableVertexAttribArray(VEC4_0_32);
			glEnableVertexAttribArray(VEC4_1_32);
			break;

		case 4:
			glEnableVertexAttribArray(VEC4_0_16);
			break;
		};

		glBufferData(
			GL_ARRAY_BUFFER,
			num_vtxs * vtx_size * sizeof(float),
			vtxs.get(),
			GL_DYNAMIC_DRAW);

		glBufferData(
			GL_ELEMENT_ARRAY_BUFFER,
			num_inds * sizeof(uint32),
			inds.get(),
			GL_DYNAMIC_DRAW);

		subRender();
		
		glDrawElements(render_type, num_inds, GL_UNSIGNED_INT, 0);

		switch (vtx_size)
		{
		case 16:
			glDisableVertexAttribArray(VEC4_0_64);
			glDisableVertexAttribArray(VEC4_1_64);
			glDisableVertexAttribArray(VEC4_2_64);
			glDisableVertexAttribArray(VEC4_3_64);
			break;

		case 8:
			glDisableVertexAttribArray(VEC4_0_32);
			glDisableVertexAttribArray(VEC4_1_32);
			break;

		case 4:
			glDisableVertexAttribArray(VEC4_0_16);
			break;
		};

		glUseProgram(0);
	};
};
