#include "Inputs.h"

#include "Mem_leak.h"

// OpenGL
#include <GLEW/glew.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
using namespace glm;
//

#include <vector>

struct Input
{
	void exec()
	{
		t(o);
	};

	inputs::Task t;
	inputs::Obj* o;
};

class Inputs
{
public:
	Inputs()
	{

	};
	~Inputs()
	{

	};

	// Keys
	void load() {

	};
	
	static void do_keys(GLFWwindow* w, int key, int scancode, int action, int mods)
	{

	};

	Inputs
		*next = nullptr,
		*prev = nullptr;
};

namespace inputs
{
	Inputs* INPUTS = nullptr;

	void update()
	{
		glfwPollEvents();
	};

	void next()
	{
		INPUTS = new Inputs();
		glfwSetWindowUserPointer(glfwGetCurrentContext(), INPUTS);
	};

	void prev()
	{

	};
};
