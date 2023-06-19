#include "Geometry.h"

#include "GLEW/glew.h"
#include "GLFW/glfw3.h"


namespace loom
{
	struct GeometryManager :
		virtual public Loadable
	{
		void load() override
		{
			Geometry::_color = glGetUniformLocation(Geometry::shader->id, "color");
			Geometry::_trns = glGetUniformLocation(Geometry::shader->id, "trns");
			Geometry::_mvp = glGetUniformLocation(Geometry::shader->id, "mvp");
		};
	};
	static inline ptr<GeometryManager> manager;


	Geometry::Geometry(auto&& collision_type, auto&& num_vtxs, auto&& num_inds, auto&& render_type)
		: Mesh(shader, 4, render_type, num_vtxs, num_inds),
		collision_type(collision_type)
	{ };
	void Geometry::subRender()
	{
		glUniform4fv(_color, 1, &color[0]);
		glUniformMatrix4fv(_trns, 1, GL_FALSE, &trns[0][0]);
		glUniformMatrix4fv(_mvp, 1, GL_FALSE, (float*)mvp);
	};


	Cube::Cube() : Geometry(collision_type, 8, 24, GL_QUADS)
	{
		allocate(0,	vec4(0, 0, 0, 1),	// 0
					vec4(1, 0, 0, 1),	// 1
					vec4(1, 1, 0, 1),	// 2
					vec4(0, 1, 0, 1),	// 3
					vec4(0, 0, 1, 1),	// 4
					vec4(1, 0, 1, 1),	// 5
					vec4(1, 1, 1, 1),	// 6
					vec4(0, 1, 1, 1));	// 7
		index(0,	vec4(3, 2, 1, 0),	// Front
					vec4(4, 5, 6, 7),	// Back
					vec4(3, 0, 4, 7),	// Left
					vec4(1, 2, 6, 5),	// Right
					vec4(0, 1, 5, 4),	// Bottom
					vec4(2, 3, 7, 6));	// Top
	};
	Rect::Rect() : Geometry(collision_type, 4, 4, GL_QUADS)
	{
		allocate(0,	vec4(0, 0, 0, 1),
					vec4(1, 0, 0, 1),
					vec4(1, 1, 0, 1),
					vec4(0, 1, 0, 1));
		index(0,	0, 1, 2, 3);
	};
	Circle::Circle() : Geometry(collision_type, 0, 0, GL_TRIANGLES)
	{
		// TODO:
	};
};
