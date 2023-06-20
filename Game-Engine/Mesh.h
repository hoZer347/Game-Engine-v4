#pragma once

#include "Data.h"
#include "Enums.h"
#include "Shader.h"

#include "GLEW/glew.h"
#include "GLFW/glfw3.h"

#include <iostream>

#include <vector>

#include <glm/glm.hpp>
using namespace glm;

namespace loom
{
	typedef unsigned int uint32;

	// Standard Mesh
	// Used to render complex 3D objects
	// Parameters:
	// - shader: the shader used to draw this
	// - render_type: the shape alignment of the vertices (i.e. triangles, lines, etc.)
	// - vtx_size: size of a single vertex by number of floats
	// - num_vtxs: number of vertices the mesh will hold
	// - num_inds: number of indices the mesh will use
	template <auto vtx_size, auto render_type, auto num_vtxs, auto num_inds>
	struct Mesh :
		virtual public Renderable
	{
		Mesh(auto&& shader) :
			shader(shader),
			vtxs((float*)malloc(num_vtxs* vtx_size * sizeof(float))),
			inds((uint32*)malloc(num_inds * sizeof(uint32)))
		{ };
		virtual ~Mesh()
		{ };

		Shader& shader;


		// Prints the contents of the mesh
		inline virtual void print()
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


		// Used to change values of vertices
		// TODO: Finish this description
		inline void modify(uint32 start_pos, auto&& element, auto&&... elements)
		{
			constexpr uint32 size = sizeof(decltype(element)) / sizeof(float);

			// TODO: Check if element has operator[]
			if constexpr (size > 1 || typeid(element) == typeid(std::vector<float>))
				for (uint32 i = 0; i < size; i++)
					vtxs[start_pos + i] = (float)element[i];
			else vtxs[start_pos] = element;

			modify(start_pos + size, elements...);
		};


		// Used to change values of indices
		// TODO: Finish this description
		inline void index(uint32 start_pos, auto&& element, auto&&... elements)
		{
			constexpr uint32 size = sizeof(decltype(element)) / sizeof(uint32);

			// TODO: Check if element has operator[]
			if constexpr (size > 1)
				for (uint32 i = 0; i < size; i++)
					inds[start_pos + i] = (uint32)element[i];
			else inds[start_pos] = element;

			index(start_pos + size, elements...);
		};


		// Combines multiple Mesh's vertices to reduce draw calls
		// TODO: Finish this description
		inline void combine(std::vector<Mesh*> meshes)
		{
			// TODO: Implement this
			// Create a new mesh with the size of all the others combined
			// Fill it with the copied data from the other meshes
			// Then make all meshes point to where their respective area is
		};


	protected:
		void combine(Mesh& mesh) { };
		inline void index(uint32 start_pos) { };
		inline void modify(uint32 start_pos) { };
		inline virtual void render()
		{
			subRender();

			shader.load();

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
		virtual void subRender() { };

		const std::shared_ptr<float[]>	vtxs;
		const std::shared_ptr<uint32[]>	inds;
	};
};
