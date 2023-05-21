#pragma once

#include "Loom.h"

#include <glm/glm.hpp>
using namespace glm;

#include <thread>
#include <atomic>
#include <memory>
#include <vector>
#include <string>
#include <iostream>

namespace loom
{
	// TODO: Move below to new file
	
	// Getting around thread_local bullshit
	struct ShaderManager;
	ShaderManager* GetSMgr();
	//


	// GameObjects
	template <typename T>
	struct GameObject
	{
	public:
		friend struct Loom;
		GameObject()
		{
			objects.push_back(static_cast<T*>(this));
		};
		virtual ~GameObject()
		{
			for (int64_t i = objects.size()-1; i >= 0; i--)
				if (objects[i] == static_cast<T*>(this))
				{
					objects[i] = objects.back();
					objects.pop_back();
					return;
				};
		};
		static inline std::vector<T*> objects;
	};
	struct Renderable : public GameObject<Renderable>
	{
	protected:
		friend struct Loom;
		friend struct Camera;
		virtual void render()=0;
		static void render_all()
		{
			for (auto& obj : objects)
				obj->render();
		};
	};
	struct Updateable : public GameObject<Updateable>
	{
	protected:
		friend struct Loom;
		virtual void update()=0;
		static void update_all()
		{
			for (auto& obj : objects)
				obj->update();
		};
	};
	struct Loadable : public GameObject<Loadable>
	{
	protected:
		friend struct Loom;
		virtual void load()=0;
		static void load_all()
		{
			size_t i = 0;
			while (i < objects.size())
			{
				objects[i]->load();
				i++;
			};
			objects.clear();
		};
	};
	struct Unloadable : public GameObject<Unloadable>
	{
	protected:
		friend struct Loom;
		virtual void unload()=0;
		static void unload_all()
		{
			size_t i = 0;
			while (i < objects.size())
			{
				objects[i]->unload();
				i++;
			};
			objects.clear();
		};
	};
	//



	// Shader Object
	struct Shader final : virtual protected Loadable, virtual protected Unloadable
	{
		uint32_t id = 0;
		
		Shader(std::string files...)
		: files({ files })
		{ };

	private:
		void load();
		void unload() { };

		std::vector<std::string> files;
	};
	//



	// Texture Object
	struct Texture final : virtual protected Loadable, virtual protected Unloadable
	{
		uint32_t id = 0;
		int32_t w = -1, h = -1;

		Texture(std::string file, uint32_t type);

	private:
		void load();
		void unload() { };

		std::function<void()> f;
		std::string file;
		uint32_t type = 0;
		int32_t nrChannels = -1;
	};
	//
};
