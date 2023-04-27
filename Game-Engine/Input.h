#pragma once

#include "Data.h"

#include <atomic>
#include <memory>
#include <functional>
#include <unordered_map>

#ifndef INPUT_TICKRATE
#define INPUT_TICKRATE 60 // Times per second input will be updated
#endif

namespace loom
{
	typedef std::function<void()> Task;
	
	struct Input final
	{	
		Input(std::shared_ptr<Input> _prev = nullptr);
		static void next();
		static void prev();

		static void MouseButtonPress(Task task, uint16_t button, uint16_t action, uint16_t mods=0);
		static void KeyPress(Task task, uint16_t key, uint16_t action, uint16_t scancode=0, uint16_t mods=0);
		static void MouseButtonHold(Task task, uint16_t button, uint16_t action, uint16_t mods=0);
		static void KeyHold(Task task, uint16_t key, uint16_t action, uint16_t scancode=0, uint16_t mods=0);
		
		static void GetMousePos(double& mx, double& my);
		static void GetRelativeMousePos(double& mx, double& my);
		static void GetScrollPos(double& sx, double& sy);
		static void GetRelativeScrollPos(double& sx, double& sy);

		static void AddTask(Task task);

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
		static inline double mx=0, my=0;		// Current mouse Position
		static inline double pmx=0, pmy=0;	// Mouse position at last tick
		static inline double sx=0, sy=0;		// Current scroll position
		static inline double psx=0, psy=0;	// Scroll Position at last tick

		std::vector<Task> tasks;
		std::vector<std::pair<input, Task>> keys;
		std::vector<std::pair<input, Task>> mbns;
		std::unordered_map<uint64_t, Task> inputs;
	};
};
