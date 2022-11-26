#include "Input.h"

#include <GLEW/glew.h>
#include <GLFW/glfw3.h>

#include "Engine.h"

static inline std::mutex mut;

namespace eng
{
	Input::Binding::Binding()
	{
		
	};

	Input::Binding::~Binding()
	{
		window.reset();
	};

	void Input::Binding::kill()
	{
		mut.lock();
		KILL = true;
		mut.unlock();
	};

	void Input::Binding::bind(Task input, Task output)
	{
		mut.lock();
		bindings.emplace_back(input, output);
		mut.unlock();
	};

	void Input::Binding::load(std::shared_ptr<Window> window)
	{
		this->window = window;

		window->assign([this]()
			{
				mut.lock();
				for (auto& binding : bindings)
					if (binding.first && (*binding.first)() && binding.second && !(*binding.second)())
						binding.second = nullptr;
				mut.unlock();

				return (bool)KILL;
			});
	};

	std::shared_ptr<Input::Binding> Input::load(std::shared_ptr<Window> window)
	{
		auto binding = Data<Input::Binding>::create();

		if (!window)
			// TODO: Assert
			return nullptr;
		
		binding->load(window);

		return binding;
	};
};
