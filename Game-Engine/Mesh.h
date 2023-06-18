#pragma once

#include "Data.h"
#include "Camera.h"

#include <mutex>
#include <atomic>
#include <vector>
#include <functional>

#include <glm/glm.hpp>
using namespace glm;

namespace loom
{
	struct Texture;
	struct Shader;
	struct Mesh;

	typedef unsigned int uint32;

	struct alignas(32) Mesh :
		public GameObject<Mesh>,
		virtual protected Renderable
	{
		// Standard Mesh
		// Used to render complex 3D objects
		// Parameters:
		// - shader: the shader used to draw this
		// - render_type: the shape alignment of the vertices (i.e. triangles, lines, etc.)
		// - vtx_size: size of a single vertex by number of floats
		// - num_vtxs: number of vertices the mesh will hold
		// - num_inds: number of indices the mesh will use
		Mesh(Shader& shader, const auto& render_type, const auto& vtx_size, auto&& num_vtxs, auto&& num_inds = 0) :
			shader(shader),
			render_type(render_type),
			vtx_size(vtx_size),
			num_vtxs(num_vtxs),
			num_inds(num_inds)
		{
			vtxs = std::shared_ptr<float[]>((float*)malloc(num_vtxs * vtx_size * sizeof(float)));
			inds = std::shared_ptr<uint32[]>((uint32*)malloc(num_inds * sizeof(uint32)));
		};
		virtual ~Mesh()
		{ };

		const uint32 num_vtxs, num_inds;
		const uint8 vtx_size;
		const uint16 render_type;

		virtual void subRender() { };

		// Used to change values of vertices
		// TODO: Finish this description
		inline void allocate(uint32 start_pos, auto&& element, auto&&... elements)
		{
			constexpr uint32 size = sizeof(decltype(element)) / sizeof(float);
			
			if constexpr (size > 1)
				for (uint32 i = 0; i < size; i++)
					vtxs[start_pos + i + offset] = element[i];
			else vtxs[start_pos + offset] = element;

			allocate(start_pos + size, elements...);
		};

		// Used to change values of indices
		// TODO: Finish this description
		inline void index(uint32 start_pos, auto&& element, auto&&... elements)
		{
			constexpr uint32 size = sizeof(decltype(element)) / sizeof(uint32);

			if constexpr (size > 1)
				for (uint32 i = 0; i < size; i++)
					inds[start_pos + i + offset] = element[i];
			else inds[start_pos + offset] = element;

			index(start_pos + size, elements...);
		};

		// Combines multiple Mesh's vertices into one reduce draw calls
		// TODO: Finish this description
		template <typename... MESHES>
		inline void combine(Mesh* receiver, MESHES*... to_combine)
		{



			combine(to_combine);
		};

		void print();

	protected:
		Shader& shader;
		std::shared_ptr<float[]>	vtxs;
		std::shared_ptr<uint32[]>	inds;

	private:
		uint32 offset=0;
		void render() override;
		void combine();
		void index(uint32 start_pos) { };
		void allocate(uint32 start_pos) { };
	};


	struct Mesh3D final :
		virtual public Mesh,
		virtual private Loadable
	{
		Mesh3D(auto&&... args) : Mesh(args...)
		{ };

		mat4 trns{ 1 };
		mat4* mvp { &Camera::mvp };

	private:
		void load() override;
		uint32 _mvp=0, _trns=0;
		void subRender() override;
	};


	struct Mesh2D final :
		virtual public Mesh
	{
		Mesh2D(auto&&... args) : Mesh(args...)
		{ };

	private:
		void subRender() override;
	};
};
