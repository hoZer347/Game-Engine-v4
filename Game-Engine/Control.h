#pragma once

#include "Data.h"

#include <array>
#include <atomic>
#include <memory>
#include <vector>
#include <functional>
#include <unordered_map>

#ifndef CONTROL_TICKRATE
#define CONTROL_TICKRATE 60 // Times per second input will be updated
#endif

#ifndef NUM_INPUTS
#define NUM_INPUTS 348
#endif

namespace loom
{
	// Standard std::function<void()>
	typedef std::function<void()> Task;

	// ID associated with a raw input
	typedef uint16 Input;

	// A function that executes on the value of a given raw input
	typedef std::function<void(float&)> Output;

	// Handles interactions between the game and the player
	// Updated at a frequency equal to CONTROL_TICKRATE
	struct Control final :
		virtual public Updateable
	{
		// Creates a control layer above the current one
		void next(Task&& task = []() {});

		// Deletes the current control layer, goes down to the previous one (can be null)
		void prev(Task&& task = []() {});

		// Resets back to layer 1
		void reset(Task&& task = []() {});
		
		// Clears current layer
		void clear();

		// Ads a task for the updater to do every frame
		void AddTask(Task&& task);

		// Adds a task to do when the "next()" function is called
		void AddOnNext(Task&& task);
		
		// Adds a task to do when the "prev()" function is called
		void AddOnPrev(Task&& task);

		// Adds tasks to do when the current input layer is reentered
		void AddOnReenter(Task&& task);

		// Adds an Output (std::function<void(float&)>) to process that state of a given input (the aformentioned float&)
		void AddOutput(Output&& output, Input&& input);

		static inline double mx = 0;	// Mouse X Position
		static inline double my = 0;	// Mouse Y Position
		static inline double pmx = 0;	// Previous Mouse X Position
		static inline double pmy = 0;	// Previous Mouse Y Position
		static inline double sx = 0;	// Scroll X Position
		static inline double sy = 0;	// Scroll Y Position

	protected:
		friend struct ControlManager;
		static inline std::array<float, NUM_INPUTS> inputs;

	private:
		void update() override;

		struct Data final
		{
			std::vector<Task> tasks;
			std::vector<Task> on_next;
			std::vector<Task> on_prev;
			std::vector<Task> on_reenter;

			std::shared_ptr<Data> _prev{0};
			std::vector<std::pair<Input, Output>> io;
		};

		std::recursive_mutex mut;
		std::shared_ptr<Data> data = std::shared_ptr<Data>(new Data());
	};
};
