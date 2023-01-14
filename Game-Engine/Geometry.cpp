#include "Geometry.h"

#include "GLEW/glew.h"
#include "GLFW/glfw3.h"

#include "Loom.h"
#include "Enums.h"
#include "Shader.h"

namespace loom
{
	static inline GLint _trns = 0;
	
	void Squares::load()
	{
		// TODO: Add Attribute Location Setting

		if (!_trns)
			_trns = glGetUniformLocation(shader.id, "trns"); glGetError();
	};
	void Squares::render()
	{
		glUseProgram(_shader.id);
		glBindTexture(GL_TEXTURE_2D, texture.id);
		glEnableVertexAttribArray(VEC4_0_32);
		glEnableVertexAttribArray(VEC4_1_32);

		glBufferData(
			GL_ARRAY_BUFFER,
			data.size() * sizeof(float),
			data.data(),
			GL_STREAM_DRAW);

		glUniformMatrix4fv(_trns, 1, GL_FALSE, &trns[0][0]); glGetError();
		glDrawArrays(GL_QUADS, 0, data.size()/8);

		glDisableVertexAttribArray(VEC4_0_32);
		glDisableVertexAttribArray(VEC4_1_32);
		glBindTexture(GL_TEXTURE_2D, 0);
		glUseProgram(0);
	};
	void Squares::unload()
	{
		glDeleteBuffers(1, &_buffer);
	};
};
