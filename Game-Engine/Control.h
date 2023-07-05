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
#define CONTROL_TICKRATE 120 // Times per second input will be updated
#endif

#ifndef NUM_INPUTS
#define NUM_INPUTS GLFW_KEY_LAST+1
#endif

namespace loom
{
	// Standard std::function<void()>
	typedef std::function<void()> Task;

	// Handles interactions between the game and the player
	// Updated at a frequency equal to CONTROL_TICKRATE
	struct Control final
	{
		// Keeps track of the states of all the inputs
		static inline std::array<int, NUM_INPUTS> inputs;

		// Creates and returns a control layer above the current one
		_NODISCARD static std::shared_ptr<Control> next();

		// Goes down a control layer
		void prev();

		// Clears current layer
		void clear();

		// Adds a task for the updater to do every frame
		void AddTask(Task&& task);

		// Adds a task to do when the "next()" function is called
		void AddOnNext(Task&& task);

		// Adds a task to do when the "prev()" function is called
		void AddOnPrev(Task&& task);

		// Adds a task to do when the current input layer is reentered
		void AddOnReenter(Task&& task);

		// Adds a task to do when the current input layer is reentered
		void AddOnLeave(Task&& task);

		static inline float mx = 0;		// Mouse X Position
		static inline float my = 0;		// Mouse Y Position
		static inline float pmx = 0;	// Previous Mouse X Position
		static inline float pmy = 0;	// Previous Mouse Y Position
		static inline float sx = 0;		// Scroll X Position
		static inline float sy = 0;		// Scroll Y Position

		~Control()
		{
			std::scoped_lock<std::recursive_mutex> lock{Control::mut};
			for (auto& task : on_leave)
				task();
			if (control.expired() && _prev)
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
		static inline std::weak_ptr<Control> control;
		static inline bool should_exit_iteration = false;

	private:
		Control() { };
		std::shared_ptr<Control> _prev{0};
	};
	struct ControlManager final :
		virtual public Updateable,
		virtual public Renderable
	{
	protected:
		friend struct ptr<ControlManager>;
		ControlManager()
		{
			Engine::DoOnMain([]()
			{
				Control::inputs.fill(0);

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
	private:
		void update() override
		{
			static Timer timer;

			if (timer.GetDiff_mls() < 1000.f / CONTROL_TICKRATE)
				return;
			timer.push(std::chrono::milliseconds(1000 / CONTROL_TICKRATE));

			std::scoped_lock<std::recursive_mutex> lock{Control::mut};

			if (!Control::control.expired())
				if (std::shared_ptr<Control> control { Control::control })
					for (auto& task : control->tasks)
						if (!Control::should_exit_iteration)
							task();
						else
						{
							Control::should_exit_iteration = false;
							break;
						};
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
					Control::inputs[i] = glfwGetMouseButton(window, i);

				for (auto i = 32; i < GLFW_KEY_LAST; i++)
					Control::inputs[i] = glfwGetKey(window, i);
			};
		};
		static inline ptr<ControlManager> manager { 0 };
	};
	inline std::shared_ptr<Control> Control::next()
	{
		std::scoped_lock<std::recursive_mutex> lock{mut};

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

		should_exit_iteration = true;

		return new_control;
	};
	inline void Control::prev()
	{
		std::scoped_lock<std::recursive_mutex> lock{mut};

		std::shared_ptr<Control> new_control { _prev };

		for (auto& task : on_prev)
			task();

		for (auto& task : on_leave)
			task();

		(*this) = (*_prev);

		for (auto& task : on_reenter)
			task();

		should_exit_iteration = true;
	};
	inline void Control::clear()
	{
		std::scoped_lock<std::recursive_mutex> lock{mut};
		tasks.clear();
		on_next.clear();
		on_prev.clear();
	};
	inline void Control::AddTask(Task&& task)
	{
		std::scoped_lock<std::recursive_mutex> lock{mut};
		tasks.emplace_back(task);
	};
	inline void Control::AddOnNext(Task&& task)
	{
		std::scoped_lock<std::recursive_mutex> lock{mut};
		on_next.emplace_back(task);
	};
	inline void Control::AddOnPrev(Task&& task)
	{
		std::scoped_lock<std::recursive_mutex> lock{mut};
		on_prev.emplace_back(task);
	};
	inline void Control::AddOnReenter(Task&& task)
	{
		std::scoped_lock<std::recursive_mutex> lock{mut};
		on_reenter.emplace_back(task);
	}
	inline void Control::AddOnLeave(Task&& task)
	{
		std::scoped_lock<std::recursive_mutex> lock{mut};
		on_leave.emplace_back(task);
	};
};
