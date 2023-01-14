#pragma once

#include <glm/glm.hpp>
using namespace glm;

#include <mutex>
#include <queue>
#include <thread>
#include <vector>
#include <string>
#include <iostream>
#include <functional>

namespace loom
{
	typedef std::function<void()> Task;

	typedef uint32_t ID;
	typedef uint32_t TYPE;

	struct ShaderManager;
	struct TextureManager;

	ShaderManager* GetSMgr();
	TextureManager* GetTMgr();

	struct Object
	{
	protected:
		friend struct Loom;
		Object();
		virtual void load() = 0;
		virtual void unload() = 0;
		static inline std::vector<Object*> objects;
	};



	struct Renderable
	{		
	protected:
		friend struct Loom;
		Renderable();
		virtual void render()=0;
		static inline std::vector<Renderable*> renderables;
	};



	struct Updatable
	{
	protected:
		friend struct Loom;
		Updatable();
		virtual void update()=0;
		static inline std::vector<Updatable*> updatables;
	};



	struct Helper final : public Object
	{
		Helper() { };

		void assign_on_kill(Task task);
		void assign(Task task);
		void push(Task task);

		void play() { STOP = false; };
		void stop() { STOP = true;  };
		void kill() { KILL = true;  };

	private:
		void load() override;
		void unload() override;

		std::mutex mut;
		std::thread thread;
		std::queue<Task> for_tasks;
		std::queue<Task> for_kernels;
		std::queue<Task> tasks;
		std::vector<Task> kernels;
		std::vector<Task> on_kill;
		std::atomic<bool> STOP = false;
		std::atomic<bool> KILL = false;
	};



	struct Shader final : public Object
	{
		Shader(std::string files...)
			: files({ files })
		{ };

		ID id = 0;

	private:
		void load() override;
		void unload() override;

		std::vector<std::string> files;
	};



	struct Texture final : public Object
	{
		Texture(std::string file, TYPE type)
		: file(file), type(type), Object()
		{ };

		ID id = 0;

	private:
		void load() override;
		void unload() override;
		
		std::string file;
		TYPE type;
	};
};
