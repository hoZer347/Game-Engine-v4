#pragma once

#include "Data.h"

#include <vector>

#include <glm/glm.hpp>
using namespace glm;

namespace loom
{
	typedef unsigned int uint32;

	struct Shader;

	// Standard Mesh
	// Used to render complex 3D objects
	// Parameters:
	// - shader: the shader used to draw this
	// - render_type: the shape alignment of the vertices (i.e. triangles, lines, etc.)
	// - vtx_size: size of a single vertex by number of floats
	// - num_vtxs: number of vertices the mesh will hold
	// - num_inds: number of indices the mesh will use
	struct Mesh :
		virtual public Renderable
	{
		Mesh(auto&& shader,
			auto&& vtx_size,
			auto&& render_type,
			auto&& num_vtxs,
			auto&& num_inds) :
			shader(shader),
			vtx_size(vtx_size),
			render_type(render_type),
			num_vtxs(num_vtxs),
			num_inds(num_inds),
			vtxs((float*)malloc(num_vtxs* vtx_size * sizeof(float))),
			inds((uint32*)malloc(num_inds * sizeof(uint32)))
		{ }
		virtual ~Mesh();

		
		Shader& shader;
		const uint8 vtx_size;
		const uint16 render_type;
		const uint32 num_vtxs, num_inds;


		// Prints the contents of the mesh
		virtual void print();


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
		static inline void combine(std::vector<Mesh*> meshes)
		{
			// TODO: Implement this
			// Create a new mesh with the size of all the others combined
			// Fill it with the copied data from the other meshes
			// Then make all meshes point to where their respective area is
		};


	private:
		void combine(Mesh& mesh) { };
		void index(uint32 start_pos) { };
		void modify(uint32 start_pos) { };

		void render() override;
		virtual void subRender() { };

		const std::shared_ptr<float[]>	vtxs;
		const std::shared_ptr<uint32[]>	inds;
	};
};
