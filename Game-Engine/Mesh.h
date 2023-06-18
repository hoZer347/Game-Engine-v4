#pragma once

#include "Camera.h"

#include <vector>
#include <functional>

#include <glm/glm.hpp>
using namespace glm;

namespace loom
{
	typedef unsigned int uint32;

	template <int8 vtx_size>
	struct Mesh :
		public GameObject<Mesh<vtx_size>>,
		virtual public Renderable
	{
		// Standard Mesh
		// Used to render complex 3D objects
		// Parameters:
		// - shader: the shader used to draw this
		// - render_type: the shape alignment of the vertices (i.e. triangles, lines, etc.)
		// - vtx_size: size of a single vertex by number of floats
		// - num_vtxs: number of vertices the mesh will hold
		// - num_inds: number of indices the mesh will use
		Mesh(ptr<Shader> shader, const uint16& render_type, uint32&& num_vtxs, uint32&& num_inds = 0);
		virtual ~Mesh();

		void print();

		static_assert(vtx_size == 4 || vtx_size == 8 || vtx_size == 16);

		ptr<Shader> shader;

	public:
		// Used to change values of vertices
		// TODO: Finish this description
		inline void allocate(uint32 start_pos, auto&& element, auto&&... elements)
		{
			constexpr uint32 size = sizeof(decltype(element)) / sizeof(float);

			if constexpr (size > 1)
				for (uint32 i = 0; i < size; i++)
					vtxs[start_pos + i] = element[i];
			else vtxs[start_pos] = element;

			allocate(start_pos + size, elements...);
		};


		// Used to change values of indices
		// TODO: Finish this description
		inline void index(uint32 start_pos, auto&& element, auto&&... elements)
		{
			constexpr uint32 size = sizeof(decltype(element)) / sizeof(uint32);

			if constexpr (size > 1)
				for (uint32 i = 0; i < size; i++)
					inds[start_pos + i] = element[i];
			else inds[start_pos] = element;

			index(start_pos + size, elements...);
		};


		// Combines multiple Mesh's vertices to reduce draw calls
		// TODO: Finish this description
		static inline void combine(std::vector<std::shared_ptr<Mesh>>&& meshes)
		{
			std::thread thread{[meshes]()
			{
				std::shared_ptr<Mesh> new_mesh;


			}};
			thread.detach();
		};


	private:
		void index(uint32 start_pos) { };
		void allocate(uint32 start_pos) { };

		void render() override;
		virtual void subRender() { };

		const uint32 num_vtxs, num_inds;
		const uint16 render_type;
		const std::shared_ptr<float[]>	vtxs;
		const std::shared_ptr<uint32[]>	inds;
	};


	template <int8 vtx_size>
	struct Mesh3D :
		virtual public Mesh<vtx_size>,
		virtual public Loadable
	{
		template <typename... ARGS>
		Mesh3D(ARGS... args) : Mesh<vtx_size>(args...)
		{ };

		mat4 trns{ 1 };
		mat4* mvp { &Camera::mvp };

	private:
		void subRender() override;
		void load() override;
		uint32 _mvp=0, _trns=0;
	};


	template <int8 vtx_size>
	struct Mesh2D :
		virtual public Mesh<vtx_size>
	{
		Mesh2D(auto&&... args) : Mesh<vtx_size>(args...)
		{ };

	private:
		void subRender() override;
	};
};
