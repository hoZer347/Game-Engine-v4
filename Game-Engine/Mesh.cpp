#include "Mesh.h"

#include "Data.h"

#include "GLEW/glew.h"
#include "GLFW/glfw3.h"

#include <iostream>

namespace loom
{
	template <int8 vtx_size>
	Mesh<vtx_size>::Mesh(
		Shader& shader,
		const uint16& render_type,
		uint32&& num_vtxs,
		uint32&& num_inds) :
		shader(shader),
		render_type(render_type),
		num_vtxs(num_vtxs),
		num_inds(num_inds),
		vtxs((float*)malloc(num_vtxs* vtx_size * sizeof(float))),
		inds((uint32*)malloc(num_inds * sizeof(uint32)))
	{ };
	template <int8 vtx_size>
	Mesh<vtx_size>::~Mesh()
	{ };


	template <int8 vtx_size>
	void Mesh<vtx_size>::print()
	{
		std::cout << "Vertices: " << std::endl;
		for (uint32 i = 0; i < num_vtxs * 4; i+=vtx_size)
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
	}
	template <int8 vtx_size>
	void Mesh<vtx_size>::render()
	{
		glUseProgram(shader.id);

		if constexpr (vtx_size == 16)
		{
			glEnableVertexAttribArray(VEC4_0_64);
			glEnableVertexAttribArray(VEC4_1_64);
			glEnableVertexAttribArray(VEC4_2_64);
			glEnableVertexAttribArray(VEC4_3_64);
		}
		else if constexpr (vtx_size == 8)
		{
			glEnableVertexAttribArray(VEC4_0_32);
			glEnableVertexAttribArray(VEC4_1_32);
		}
		else if constexpr (vtx_size == 4)
		{
			glEnableVertexAttribArray(VEC4_0_16);
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

		if constexpr (vtx_size == 16)
		{
			glDisableVertexAttribArray(VEC4_0_64);
			glDisableVertexAttribArray(VEC4_1_64);
			glDisableVertexAttribArray(VEC4_2_64);
			glDisableVertexAttribArray(VEC4_3_64);
		}
		else if constexpr (vtx_size == 8)
		{
			glDisableVertexAttribArray(VEC4_0_32);
			glDisableVertexAttribArray(VEC4_1_32);
		}
		else if constexpr (vtx_size == 4)
		{
			glDisableVertexAttribArray(VEC4_0_16);
		};
	};


	template <int8 vtx_size>
	void Mesh3D<vtx_size>::load()
	{
		_mvp = glGetUniformLocation(Mesh<vtx_size>::shader.id, "mvp");
		_trns = glGetUniformLocation(Mesh<vtx_size>::shader.id, "trns");
	};
	template <int8 vtx_size>
	void Mesh3D<vtx_size>::subRender()
	{
		glUniformMatrix4fv(_trns, 1, GL_FALSE, &trns[0][0]);
		glUniformMatrix4fv(_mvp, 1, GL_FALSE, (float*)mvp);
	};
	template <int8 vtx_size>
	void Mesh2D<vtx_size>::subRender()
	{

	};

	template struct Mesh<4u>;
	template struct Mesh<8u>;
	template struct Mesh<16u>;

	template struct Mesh2D<4u>;
	template struct Mesh2D<8u>;
	template struct Mesh2D<16u>;

	template struct Mesh3D<4u>;
	template struct Mesh3D<8u>;
	template struct Mesh3D<16u>;
};
