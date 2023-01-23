#pragma once

#include "Data.h"

#include <atomic>
#include <functional>
#include <unordered_map>

namespace loom
{
	typedef std::function<void()> Task;

	struct Input final
	{	
		Input(std::shared_ptr<Input> _prev = nullptr);
		static void next();
		static void prev();
		static void MouseButtonPress(Task task, uint16_t button, uint16_t action, uint16_t mods=0);
		static void KeyPress(Task task, uint16_t key, uint16_t scancode, uint16_t action, uint16_t mods=0);
		static void MouseButtonHold(Task task, uint16_t button, uint16_t action, uint16_t mods = 0);
		static void KeyHold(Task task, uint16_t key, uint16_t scancode, uint16_t action, uint16_t mods = 0);

	protected:
		friend struct Loom;
		friend class std::shared_ptr<Input>;
		std::shared_ptr<Input> _prev = nullptr;
		static void load();
		static void update();

		union input
		{
			uint16_t data[4];
			uint64_t input;
		};

	private:
		Input() { };
		std::vector<std::pair<input, Task>> keys;
		std::vector<std::pair<input, Task>> mbns;
		std::unordered_map<uint64_t, Task> inputs;
	};
};
