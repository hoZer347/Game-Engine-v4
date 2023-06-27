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
		_NODISCARD static std::shared_ptr<Control> next(Task&& task = []() {});

		// Deletes the current control layer, goes down to the previous one
		void prev(Task&& task = []() {});

		// Resets back to given layer (default nullptr)
		static void reset(std::shared_ptr<Control> new_control, Task&& task = []() {});

		// Clears current layer
		static void clear();

		// Does a task with the guarantee that the curreny control layer will be maintained
		static void DoOnCurrentLayer(Task&& task);

		// Adds a task for the updater to do every frame
		static void AddTask(Task&& task);

		// Adds a task to do when the "next()" function is called
		static void AddOnNext(Task&& task);

		// Adds a task to do when the "prev()" function is called
		static void AddOnPrev(Task&& task);

		// Adds a task to do when the current input layer is reentered
		static void AddOnReenter(Task&& task);

		// Adds a task to do when the current input layer is reentered
		static void AddOnLeave(Task&& task);

		static inline float mx = 0;		// Mouse X Position
		static inline float my = 0;		// Mouse Y Position
		static inline float pmx = 0;	// Previous Mouse X Position
		static inline float pmy = 0;	// Previous Mouse Y Position
		static inline float sx = 0;		// Scroll X Position
		static inline float sy = 0;		// Scroll Y Position

		~Control()
		{
			control = _prev;
		};

	protected:
		friend struct ControlManager;

		std::vector<Task> tasks;
		std::vector<Task> on_next;
		std::vector<Task> on_prev;
		std::vector<Task> on_reenter;
		std::vector<Task> on_leave;

		static inline std::recursive_mutex mut;
		static inline std::array<int, NUM_INPUTS> prev_inputs;
		static inline std::weak_ptr<Control> control;

	private:
		Control() { };
		static inline bool same_layer_guarantee = false;
		std::shared_ptr<Control> _prev{0};
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

			if (!Control::control.expired())
				for (auto& task : std::shared_ptr<Control>(Control::control)->tasks)
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
	inline std::shared_ptr<Control> Control::next(Task&& task)
	{
		std::scoped_lock<std::recursive_mutex> lock{mut};
		
		if (same_layer_guarantee)
			return nullptr;
		same_layer_guarantee = true;

		std::shared_ptr<Control> new_control = std::shared_ptr<Control>(new Control());

		if (!control.expired())
		{
			std::shared_ptr<Control> control { Control::control };

			for (auto& task : control->on_next)
				task();

			for (auto& task : control->on_leave)
				task();

			new_control->_prev = control;
		};
		
		control = new_control;

		task();

		same_layer_guarantee = false;

		return new_control;
	};
	inline void Control::prev(Task&& task)
	{
		std::scoped_lock<std::recursive_mutex> lock{mut};

		same_layer_guarantee = true;

		std::shared_ptr<Control> new_control { _prev };

		for (auto& task : on_prev)
			task();

		for (auto& task : on_leave)
			task();

		(*this) = (*_prev);

		for (auto& task : on_reenter)
			task();

		task();

		same_layer_guarantee = false;
	};
	inline void Control::DoOnCurrentLayer(Task&& task)
	{
		std::scoped_lock<std::recursive_mutex> lock{mut};

		same_layer_guarantee = true;
		
		task();
		
		same_layer_guarantee = false;
	};
	inline void Control::reset(std::shared_ptr<Control> new_control, Task&& task)
	{
		std::scoped_lock<std::recursive_mutex> lock{mut};
		
		same_layer_guarantee = true;
		
		if (new_control)
			for (auto& task : new_control->on_leave)
				task();

		control = new_control;
		
		if (new_control)
			for (auto& task : new_control->on_reenter)
				task();

		task();

		same_layer_guarantee = false;
	};
	inline void Control::clear()
	{
		std::scoped_lock<std::recursive_mutex> lock{mut};
		std::shared_ptr<Control> control { Control::control };
		control->tasks.clear();
		control->on_next.clear();
		control->on_prev.clear();
	};
	inline void Control::AddTask(Task&& task)
	{
		std::scoped_lock<std::recursive_mutex> lock{mut};
		if (!same_layer_guarantee)
			std::cout << "Task added outside of .next(), .prev() or .DoOnCurrentLayer()" << std::endl;
		std::shared_ptr<Control> control { Control::control };
		control->tasks.emplace_back(task);
	};
	inline void Control::AddOnNext(Task&& task)
	{
		std::scoped_lock<std::recursive_mutex> lock{mut};
		if (!same_layer_guarantee)
			std::cout << "Task added outside of .next(), .prev() or .DoOnCurrentLayer()" << std::endl;
		std::shared_ptr<Control> control { Control::control };
		control->on_next.emplace_back(task);
	};
	inline void Control::AddOnPrev(Task&& task)
	{
		std::scoped_lock<std::recursive_mutex> lock{mut};
		if (!same_layer_guarantee)
			std::cout << "Task added outside of .next(), .prev() or .DoOnCurrentLayer()" << std::endl;
		std::shared_ptr<Control> control { Control::control };
		control->on_prev.emplace_back(task);
	};
	inline void Control::AddOnReenter(Task&& task)
	{
		std::scoped_lock<std::recursive_mutex> lock{mut};
		if (!same_layer_guarantee)
			std::cout << "Task added outside of .next(), .prev() or .DoOnCurrentLayer()" << std::endl;
		std::shared_ptr<Control> control { Control::control };
		control->on_reenter.emplace_back(task);
	}
	inline void Control::AddOnLeave(Task&& task)
	{
		std::scoped_lock<std::recursive_mutex> lock{mut};
		if (!same_layer_guarantee)
			std::cout << "Task added outside of .next(), .prev() or .DoOnCurrentLayer()" << std::endl;
		std::shared_ptr<Control> control { Control::control };
		control->on_leave.emplace_back(task);
	};
};
