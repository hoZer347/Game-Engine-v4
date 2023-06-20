#pragma once

#include "../Data.h"
#include "../Mesh.h"
#include "../Shader.h"

namespace loom
{
	template <uint16 x_size, uint16 y_size>
	struct Map :
		public Mesh<4, GL_QUADS, x_size * y_size, x_size * y_size * Mesh::vtx_size>,
		virtual public Updateable,
		virtual public Renderable
	{
		Map();

		void update() override;
		void render() override;




		static inline Shader shader{ "Map" };
	};
};





namespace loom
{
	template<uint16 x_size, uint16 y_size>
	inline Map<x_size, y_size>::Map() :
		Mesh(shader, 4, GL_QUADS, x_size, y_size)
	{

	};
	template<uint16 x_size, uint16 y_size>
	inline void Map<x_size, y_size>::update()
	{

	};
	template<uint16 x_size, uint16 y_size>
	inline void Map<x_size, y_size>::render()
	{

	};
};
