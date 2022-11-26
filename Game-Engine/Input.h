#pragma once

#include "Task.h"
#include "Data.h"

#include <atomic>
#include <memory>
#include <map>

namespace eng
{
	struct Thread;
	struct Window;

	struct Input
	{
	private:
		Input() { };

		struct Binding
		{
			~Binding();

			void kill();
			void bind(Task input, Task output);

		protected:
			friend struct Data<Binding>;
			friend struct Input;
			Binding();
			void load(std::shared_ptr<Window> window);

		private:
			std::mutex							mut;
			std::atomic<bool>					KILL = false;
			std::vector<std::pair<Task, Task>>	bindings;
			std::shared_ptr<Window>				window;
		};

	public:
		static std::shared_ptr<Binding> load(std::shared_ptr<Window> window);
	};
};
