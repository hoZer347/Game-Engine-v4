#pragma once

#include "Data.h"
#include "Timing.h"

#include "GLEW/glew.h"
#include "GLFW/glfw3.h"

#include <array>
#include <atomic>
#include <memory>
#include <vector>
#include <fstream>
#include <functional>
#include <unordered_map>

#ifndef CONTROL_TICKRATE
#define CONTROL_TICKRATE 60 // Times per second input will be updated
#endif

#ifndef NUM_INPUTS
#define NUM_INPUTS GLFW_KEY_LAST+1
#endif

namespace loom
{
	// Standard std::function<void()>
	typedef std::function<void()> Task;
	
	// Keeps track of the states of all the inputs
	inline std::array<int, NUM_INPUTS> inputs;

	// Handles interactions between the game and the player
	// Updated at a frequency equal to CONTROL_TICKRATE
	struct Control final
	{
		// Creates a control layer above the current one
		static void next(Task&& task = []() {});

		// Deletes the current control layer, goes down to the previous one
		static void prev(Task&& task = []() {});

		// Resets back to layer 1
		static void reset(Task&& task = []() {});

		// Clears current layer
		static void clear();

		// Adds a task for the updater to do every frame
		static void AddTask(Task&& task);

		// Adds a task to do when the "next()" function is called
		static void AddOnNext(Task&& task);

		// Adds a task to do when the "prev()" function is called
		static void AddOnPrev(Task&& task);

		// Adds tasks to do when the current input layer is reentered
		static void AddOnReenter(Task&& task);

		// Adds tasks to do when the current input layer is reentered
		static void AddOnLeave(Task&& task);

		static inline float mx = 0;		// Mouse X Position
		static inline float my = 0;		// Mouse Y Position
		static inline float pmx = 0;	// Previous Mouse X Position
		static inline float pmy = 0;	// Previous Mouse Y Position
		static inline float sx = 0;		// Scroll X Position
		static inline float sy = 0;		// Scroll Y Position

	protected:
		friend struct ControlManager;

		std::vector<Task> tasks;
		std::vector<Task> on_next;
		std::vector<Task> on_prev;
		std::vector<Task> on_reenter;
		std::vector<Task> on_leave;

		std::shared_ptr<Control> _prev{0};

		static inline std::recursive_mutex mut;
		static inline std::array<int, NUM_INPUTS> prev_inputs;
		static inline std::shared_ptr<Control> control = nullptr;

	private:
		Control() { };
	};
	struct ControlManager final :
		virtual public Updateable,
		virtual public Renderable
	{
		ControlManager()
		{
			Engine::DoOnMain([]()
			{
				inputs.fill(0);

				if (GLFWwindow* window = glfwGetCurrentContext())
				{
					glfwSetScrollCallback(window, [](GLFWwindow* window, double _sx, double _sy)
					{
						Control::sx += (float)_sx;
						Control::sy += (float)_sy;
					});
					glfwSetWindowSizeCallback(window, [](GLFWwindow*, int w, int h)
					{
						glViewport(0, 0, w, h);
					});
				};
			});
		};
		void update() override
		{
			static Timer timer;

			if (timer.GetDiff_mls() < 1000.f / CONTROL_TICKRATE)
				return;
			timer.push(std::chrono::milliseconds(1000 / CONTROL_TICKRATE));

			std::scoped_lock<std::recursive_mutex> lock{Control::mut};

			for (auto& task : Control::control->tasks)
				task();

			inputs.swap(Control::prev_inputs);
			Control::prev_inputs.fill(0);
		};
		void render() override
		{
			static Timer timer;

			if (timer.GetDiff_mls() < 1000.f / CONTROL_TICKRATE)
				return;
			timer.push(std::chrono::milliseconds(1000 / CONTROL_TICKRATE));

			if (GLFWwindow* window = glfwGetCurrentContext())
			{
				double _mx, _my;
				glfwGetCursorPos(window, &_mx, &_my);
				Control::pmx = Control::mx;
				Control::pmy = Control::my;
				Control::mx = (float)_mx;
				Control::my = (float)_my;

				glfwPollEvents();

				for (auto i = 0; i < GLFW_MOUSE_BUTTON_LAST; i++)
					Control::prev_inputs[i] = glfwGetMouseButton(window, i);

				for (auto i = 32; i < GLFW_KEY_LAST; i++)
					Control::prev_inputs[i] = glfwGetKey(window, i);
			};
		};

		static inline ptr<ControlManager> manager { 1 };
	};
	inline void Control::next(Task&& task)
	{
		std::scoped_lock<std::recursive_mutex> lock{mut};
		
		static bool _next = false;
		if (_next)
			return;
		_next = true;

		if (control)
			for (auto& task : control->on_next)
				task();

		if (control)
			for (auto& task : control->on_leave)
				task();

		std::shared_ptr<Control> new_control = std::shared_ptr<Control>(new Control());
		new_control->_prev = control;
		control = new_control;

		if (control)
			for (auto& task : control->on_reenter)
				task();

		task();

		_next = false;
	};
	inline void Control::prev(Task&& task)
	{
		std::scoped_lock<std::recursive_mutex> lock{mut};

		for (auto& task : control->on_prev)
			task();

		if (control)
			for (auto& task : control->on_leave)
				task();

		control = control->_prev;

		for (auto& task : control->on_reenter)
			task();

		task();
	};
	inline void Control::reset(Task&& task)
	{
		std::scoped_lock<std::recursive_mutex> lock{mut};

		control = std::shared_ptr<Control>(new Control());

		task();
	};
	inline void Control::clear()
	{
		std::scoped_lock<std::recursive_mutex> lock{mut};

		control->tasks.clear();
		control->on_next.clear();
		control->on_prev.clear();
	};
	inline void Control::AddTask(Task&& task)
	{
		std::scoped_lock<std::recursive_mutex> lock{mut};
		control->tasks.emplace_back(task);
	};
	inline void Control::AddOnNext(Task&& task)
	{
		std::scoped_lock<std::recursive_mutex> lock{mut};
		control->on_next.emplace_back(task);
	};
	inline void Control::AddOnPrev(Task&& task)
	{
		std::scoped_lock<std::recursive_mutex> lock{mut};
		control->on_prev.emplace_back(task);
	};
	inline void Control::AddOnReenter(Task&& task)
	{
		std::scoped_lock<std::recursive_mutex> lock{mut};
		control->on_reenter.emplace_back(task);
	}
	inline void Control::AddOnLeave(Task&& task)
	{
		std::scoped_lock<std::recursive_mutex> lock{mut};
		control->on_leave.emplace_back(task);
	};
};
