#include "Geometry.h"

#include "GLEW/glew.h"
#include "GLFW/glfw3.h"

#include "Data.h"
#include "Utils.h"
#include "Enums.h"
#include "Camera.h"


namespace loom
{
	void Cube::load()
	{
		// This can be improved (called once only)
		_color = glGetUniformLocation(shader.id, "color");
		_trns = glGetUniformLocation(shader.id, "trns");
		_mvp = glGetUniformLocation(shader.id, "mvp");
	};
	void Cube::render()
	{
		glUseProgram(shader.id);
		glEnableVertexAttribArray(VEC4_0_16);

		glUniform4fv(_color, 1, &color[0]);
		glUniformMatrix4fv(_trns, 1, GL_FALSE, &trns[0][0]);
		glUniformMatrix4fv(_mvp, 1, GL_FALSE, &Camera::mvp[0][0]);

		glBufferData(
			GL_ARRAY_BUFFER,
			vtxs.size() * sizeof(vec4),
			vtxs.data(),
			GL_STATIC_DRAW);

		glBufferData(
			GL_ELEMENT_ARRAY_BUFFER,
			inds.size() * sizeof(unsigned int),
			inds.data(),
			GL_STATIC_DRAW);

		glDrawElements(GL_QUADS, (GLsizei)inds.size(), GL_UNSIGNED_INT, nullptr);

		glDisableVertexAttribArray(VEC4_0_16);
		glUseProgram(0);
	};


	void Square::load()
	{
		// This can be improved (called once only)
		_color = glGetUniformLocation(shader.id, "color");
		_trns = glGetUniformLocation(shader.id, "trns");
		_mvp = glGetUniformLocation(shader.id, "mvp");
	};
	void Square::render()
	{
		glUseProgram(shader.id);
		glEnableVertexAttribArray(VEC4_0_16);

		glUniform4fv(_color, 1, &color[0]);
		glUniformMatrix4fv(_trns, 1, GL_FALSE, &trns[0][0]);
		glUniformMatrix4fv(_mvp, 1, GL_FALSE, &Camera::mvp[0][0]);

		glBufferData(
			GL_ARRAY_BUFFER,
			vtxs.size() * sizeof(vec4),
			vtxs.data(),
			GL_STATIC_DRAW);

		glDrawArrays(GL_QUADS, 0, 4);

		glDisableVertexAttribArray(VEC4_0_16);
		glUseProgram(0);
	};


	void Circle::load()
	{

	};
	void Circle::render()
	{

	};
};
