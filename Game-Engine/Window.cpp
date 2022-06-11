#include "Window.h"

#include "Mem_Leak.h"

// OpenGL
#include <GLEW/glew.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
using namespace glm;
//

#include "Game.h"
#include "Inputs.h"
#include "Mesh.h"
#include "Object.h"
#include "Objects.h"

#include "Shader.h"

#include <iostream>

Window::Window(Inputs* i, Objects* o, Game* g)
{
	game = g;
	inputs = i;
	objects = o;
};

Window::~Window()
{

};

void render(Object* o)
{
	glUseProgram(o->m->shader);

	glBufferData(
		GL_ARRAY_BUFFER,
		o->m->vtxs.size() * sizeof(float),
		o->m->vtxs.data(),
		GL_DYNAMIC_DRAW);

	glBufferData(
		GL_ELEMENT_ARRAY_BUFFER,
		o->m->inds.size() * sizeof(size_t),
		o->m->inds.data(),
		GL_DYNAMIC_DRAW);

	glBufferData(
		GL_TEXTURE_BUFFER,
		o->m->texs.size() * sizeof(unsigned int),
		o->m->texs.data(),
		GL_DYNAMIC_DRAW);

	glBufferData(
		GL_UNIFORM_BUFFER,
		o->m->unfs.size() * sizeof(unsigned int),
		o->m->unfs.data(),
		GL_DYNAMIC_DRAW);
	
	glDrawElements(GL_QUADS, (GLsizei)o->m->vtxs.size(), GL_UNSIGNED_INT, 0);
};

void Window::prex()
{
	glfwInit();
	w = glfwCreateWindow(640, 640, "", nullptr, nullptr);
	glfwMakeContextCurrent(w);
	glfwSwapInterval(0);
	glewExperimental = true;
	glewInit();

	glGenBuffers(1, &_vtxs);
	glGenBuffers(1, &_inds);
	glGenBuffers(1, &_texs);
	glGenBuffers(1, &_unfs);
	
	glBindBuffer(GL_ARRAY_BUFFER, _vtxs);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _inds);
	glBindBuffer(GL_TEXTURE_BUFFER, _texs);
	glBindBuffer(GL_UNIFORM_BUFFER, _unfs);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 3, nullptr);
	glEnableVertexAttribArray(0);

	glClearColor(.5, .5, .5, 0);

	objects->add(
		new Mesh({
			{ 0, 0, 0,	1, 0, 0,	1, 1, 0,	0, 1, 0 },
			{ 0,		1,			2,			3 },
			{},
			{}, shader::create("default")
			})
	);
}

void Window::exec()
{
	// Inputs
	glfwPollEvents();

	inputs->update();

	if (glfwWindowShouldClose(w)) game->end();
	//

	// Rendering
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	
	glfwGetWindowSize(w, &width, &height);
	glViewport(0, 0, width, height);

	objects->get(render);

	glfwSwapBuffers(w);
	//

	std::cout << glewGetErrorString(glGetError()) << std::endl;

	stop();
};

void Window::pstx()
{
	objects->get([](Object* o) { delete o; });

	glDeleteBuffers(1, &_vtxs);
	glDeleteBuffers(1, &_inds);
	glDeleteBuffers(1, &_texs);
	glDeleteBuffers(1, &_unfs);

	glfwDestroyWindow(w);

	glfwTerminate();
}
