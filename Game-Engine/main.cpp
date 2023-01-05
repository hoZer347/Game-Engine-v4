#include "GLEW/glew.h"
#include "GLFW/glfw3.h"

#include "loom.h"
using namespace loom;

#define STB_IMAGE_IMPLEMENTATION

#include "Shader.hpp"
#include "Texture.hpp"

#include <glm/glm.hpp>
#include <glm/gtx/transform.hpp>
using namespace glm;

#include <iostream>

int main()
{
	Init();

	Mesh mesh;

	ShaderManager s_mgr;
	TextureManager t_mgr;

	Make<Shader>(mesh).data = s_mgr.create("shaders/default");
	Make<Texture>(mesh).data = t_mgr.create("Resources/DK.png", GL_RGBA);
	Make<Draw>(mesh).data = GL_QUADS;

	Vtxs& vtxs = Make<Vtxs>(mesh);

	vtxs.data =
	{
		{ vec4(-1, -1, 0, 1), vec4(1), vec4(1), vec4(0, 0, 0, 1), },
		{ vec4( 1, -1, 0, 1), vec4(1), vec4(1), vec4(1, 0, 0, 1), },
		{ vec4( 1,  1, 0, 1), vec4(1), vec4(1), vec4(1, 1, 0, 1), },
		{ vec4(-1,  1, 0, 1), vec4(1), vec4(1), vec4(0, 1, 0, 1), },
	};

	Inds& inds = Make<Inds>(mesh);

	inds.data = { 0, 1, 2, 3 };

	glVertexAttribPointer(
		0,
		4,
		GL_FLOAT,
		GL_FALSE,
		sizeof(Vtx),
		(void*)(0 * sizeof(vec4)));

	glVertexAttribPointer(
		1,
		4,
		GL_FLOAT,
		GL_FALSE,
		sizeof(Vtx),
		(void*)(1 * sizeof(vec4)));

	glVertexAttribPointer(
		2,
		4,
		GL_FLOAT,
		GL_FALSE,
		sizeof(Vtx),
		(void*)(2 * sizeof(vec4)));

	glVertexAttribPointer(
		3,
		4,
		GL_FLOAT,
		GL_FALSE,
		sizeof(Vtx),
		(void*)(3 * sizeof(vec4)));

	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);
	glEnableVertexAttribArray(2);
	glEnableVertexAttribArray(3);

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	glEnable(GL_MULTISAMPLE);
	glfwWindowHint(GLFW_SAMPLES, 4);

	glEnable(GL_DEBUG_OUTPUT);

	Render();

	Exit();

	return 0;
};
