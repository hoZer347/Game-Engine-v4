#include "Input.h"

#include "Engine.h"

#include <GLFW/glfw3.h>

#include <unordered_map>
#include <mutex>

std::mutex load_inputs_one_at_a_time;

namespace eng {
	namespace inp
	{
		struct InputsData
		{
			GLFWwindow* window = glfwGetCurrentContext();

			std::unordered_map<unsigned int, Task*> inputs;

			InputsData	*next = nullptr,
						*prev = nullptr;
		};

		DEFINE_TASK(Inputs, InputsData*, )
		{
			unsigned int i = 0;
		};

		static inline std::atomic<InputsData*> CURR = nullptr;

		void assign(Thread* thread)
		{
			thread->assign(Inputs::create(new InputsData()).release());
		};

		void bind(unsigned int type, Task* task)
		{
			std::scoped_lock<std::mutex> guard(load_inputs_one_at_a_time);

			CURR.load()->inputs[type] = task;
		};

		void next()
		{
			InputsData* i = new InputsData();

			if (CURR)
			{
				CURR.load()->next = i;
				CURR.load()->next->prev = CURR.load();
				CURR = CURR.load()->next;
			}
			else
			{
				CURR = i;
			}
		};

		void prev()
		{
			if (!CURR)
				return;

			if (CURR.load()->prev)
			{
				CURR = CURR.load()->prev;
				delete CURR.load()->next;
				CURR.load()->next = nullptr;
			}
			else
			{
				delete CURR;
				CURR = nullptr;
			}
		};
	};
};
