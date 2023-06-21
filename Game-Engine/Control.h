#pragma once

#include "Data.h"
#include "Utils.h"

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

		// Adds a task for the updater to do every frame
		void AddTask(Task&& task);

		// Adds a task to do when the "next()" function is called
		void AddOnNext(Task&& task);

		// Adds a task to do when the "prev()" function is called
		void AddOnPrev(Task&& task);

		// Adds tasks to do when the current input layer is reentered
		void AddOnReenter(Task&& task);

		static inline double mx = 0;	// Mouse X Position
		static inline double my = 0;	// Mouse Y Position
		static inline double pmx = 0;	// Previous Mouse X Position
		static inline double pmy = 0;	// Previous Mouse Y Position
		static inline double sx = 0;	// Scroll X Position
		static inline double sy = 0;	// Scroll Y Position

		static inline std::array<int, NUM_INPUTS> inputs;

	protected:
		friend struct ControlManager;
		static inline std::array<int, NUM_INPUTS> prev_inputs;

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


	struct ControlManager :
		virtual public Renderable
	{
		ControlManager();
		void render() override;
	};
	inline ptr<ControlManager> control_manager;
};




namespace loom
{
	inline ControlManager::ControlManager()
	{
		Engine::DoOnMain([]()
		{
			Control::inputs.fill(0);

			if (GLFWwindow* window = glfwGetCurrentContext())
			{
				glfwSetScrollCallback(window, [](GLFWwindow* window, double _sx, double _sy)
				{
					Control::sx += _sx;
					Control::sy += _sy;
				});
				glfwSetWindowSizeCallback(window, [](GLFWwindow*, int w, int h)
				{
					glViewport(0, 0, w, h);
				});
				glfwSetKeyCallback(window, [](GLFWwindow*, int button, int, int action, int)
				{
					Control::inputs[button] = action;
				});
				glfwSetMouseButtonCallback(window, [](GLFWwindow*, int button, int action, int)
				{
					Control::inputs[button] = action;
				});
			};
		});
	};
	inline void ControlManager::render()
	{
		static Utils::Timer timer;

		if (timer.GetDiff_mls() < 1000.f / CONTROL_TICKRATE)
			return;
		timer.push(std::chrono::milliseconds(1000 / CONTROL_TICKRATE));

		if (GLFWwindow* window = glfwGetCurrentContext())
		{
			Control::pmx = Control::mx;
			Control::pmy = Control::my;
			glfwGetCursorPos(window, &Control::mx, &Control::my);
			glfwPollEvents();

			for (auto i = 0; i < GLFW_MOUSE_BUTTON_LAST; i++)
				Control::prev_inputs[i] = glfwGetMouseButton(window, i);

			for (auto i = 32; i < GLFW_KEY_LAST; i++)
				Control::prev_inputs[i] = glfwGetKey(window, i);
		};
	};
	inline void Control::update()
	{
		static Utils::Timer timer;

		if (timer.GetDiff_mls() < 1000.f / CONTROL_TICKRATE)
			return;
		timer.push(std::chrono::milliseconds(1000 / CONTROL_TICKRATE));

		for (auto& task : data->tasks)
			task();
		inputs.fill(0);
		inputs.swap(prev_inputs);
	};
	inline void Control::next(Task&& task)
	{
		std::scoped_lock<std::recursive_mutex> lock{mut};

		if (data)
			for (auto& task : data->on_next)
				task();

		std::shared_ptr<Data> data = std::shared_ptr<Data>(new Data());
		data->_prev = this->data;
		this->data = data;

		task();
	};
	inline void Control::prev(Task&& task)
	{
		std::scoped_lock<std::recursive_mutex> lock{mut};

		for (auto& task : data->on_prev)
			task();

		data = data->_prev;

		for (auto& task : data->on_reenter)
			task();

		task();
	};
	inline void Control::reset(Task&& task)
	{
		std::scoped_lock<std::recursive_mutex> lock{mut};

		data = std::shared_ptr<Data>(new Data());

		task();
	};
	inline void Control::clear()
	{
		std::scoped_lock<std::recursive_mutex> lock{mut};

		data->tasks.clear();
		data->on_next.clear();
		data->on_prev.clear();
		data->io.clear();
	};
	inline void Control::AddTask(Task&& task)
	{
		std::scoped_lock<std::recursive_mutex> lock{mut};
		data->tasks.emplace_back(task);
	};
	inline void Control::AddOnNext(Task&& task)
	{
		std::scoped_lock<std::recursive_mutex> lock{mut};
		data->on_next.emplace_back(task);
	};
	inline void Control::AddOnPrev(Task&& task)
	{
		std::scoped_lock<std::recursive_mutex> lock{mut};
		data->on_prev.emplace_back(task);
	};
	inline void Control::AddOnReenter(Task&& task)
	{
		std::scoped_lock<std::recursive_mutex> lock{mut};
		data->on_reenter.emplace_back(task);
	};
};
