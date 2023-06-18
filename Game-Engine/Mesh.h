#pragma once

#include "Camera.h"

#include <vector>
#include <functional>

#include <glm/glm.hpp>
using namespace glm;

namespace loom
{
	typedef unsigned int uint32;

	struct Mesh :
		public GameObject<Mesh>,
		virtual protected Renderable
	{
	protected:
		// Standard Mesh
		// Used to render complex 3D objects
		// Parameters:
		// - shader: the shader used to draw this
		// - render_type: the shape alignment of the vertices (i.e. triangles, lines, etc.)
		// - vtx_size: size of a single vertex by number of floats
		// - num_vtxs: number of vertices the mesh will hold
		// - num_inds: number of indices the mesh will use
		Mesh(Shader& shader, const uint16& render_type, const uint8& vtx_size, uint32&& num_vtxs, uint32&& num_inds = 0);
		virtual ~Mesh();

	public:
		Shader& shader;
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

		void print();

	private:
		void render() override;
		void index(uint32 start_pos) { };
		void allocate(uint32 start_pos) { };

		uint32 offset = 0;
		const std::shared_ptr<float[]>	vtxs;
		const std::shared_ptr<uint32[]>	inds;
	};


	struct Mesh3D final :
		virtual public Mesh,
		virtual private Loadable
	{
		mat4 trns{ 1 };
		mat4* mvp { &Camera::mvp };

	protected:
		friend struct Engine;
		Mesh3D(auto&&... args) : Mesh(args...)
		{ };

	private:
		void load() override;
		uint32 _mvp=0, _trns=0;
		void subRender() override;
	};


	struct Mesh2D final :
		virtual public Mesh
	{
	protected:
		friend struct Engine;
		Mesh2D(auto&&... args) : Mesh(args...)
		{ };

	private:
		void subRender() override;
	};
};
