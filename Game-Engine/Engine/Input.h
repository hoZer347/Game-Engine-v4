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


	union Input
	{
		struct
		{
			uint16_t
				key = 0,
				button = 0,
				action = 0,
				mods = 0;
		} data;
		uint64_t input;

		operator uint64_t() { return input; }
	};


	// Singly linked list of input layers and associations
	struct Inputs final
	{	
		Inputs(std::shared_ptr<Inputs> _prev = nullptr);
		static void next();
		static void prev();

		static void MouseButtonPress(Task&& task, Input&& _input);
		static void KeyPress(Task&& task, Input&& _input);
		static void MouseButtonHold(Task&& task, Input&& _input);
		static void KeyHold(Task&& task, Input&& _input);
		
		static void GetMousePos(double& mx, double& my);
		static void GetRelativeMousePos(double& mx, double& my);
		static void GetScrollPos(double& sx, double& sy);
		static void GetRelativeScrollPos(double& sx, double& sy);

		static void clear();

	protected:
		friend struct Loom;
		friend class std::shared_ptr<Inputs>;
		std::shared_ptr<Inputs> _prev = nullptr;
		static void load();
		static void update();

	private:
		static inline double mx=0, my=0;	// Current mouse Position
		static inline double pmx=0, pmy=0;	// Mouse position at last tick
		static inline double sx=0, sy=0;	// Current scroll position
		static inline double psx=0, psy=0;	// Scroll Position at last tick

		std::vector<std::pair<Input, Task>> keys;
		std::vector<std::pair<Input, Task>> mbns;
		std::unordered_map<uint64_t, Task> inputs;
	};
};
