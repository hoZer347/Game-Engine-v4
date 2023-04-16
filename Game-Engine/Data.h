#pragma once

#include "Loom.h"

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
	struct TextureManager;
	ShaderManager* GetSMgr();
	TextureManager* GetTMgr();
	//



	// GameObjects
	template <typename T>
	struct GameObject
	{
	protected:
		friend struct Loom;
		GameObject()
		{
			objects.push_back(static_cast<T*>(this));
		};
		virtual ~GameObject()
		{
			for (auto i = 0; i < objects.size(); i++)
			{
				if (objects[i] == this)
					objects[i] = objects.back();

				objects.pop_back();
			};
		};
		static inline std::vector<T*> objects;
	};
	struct Renderable : public GameObject<Renderable>
	{
	protected:
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
			// Accounting for a loadable adding another loadable
			size_t i = 0;
			while (i < objects.size())
			{
				objects[i]->load();
				i++;
			};
			objects.clear();
			//
		};
	};
	struct Unloadable : public GameObject<Unloadable>
	{
	protected:
		friend struct Loom;
		virtual void unload()=0;
		static void unload_all()
		{
			// Accounting for an unloadable adding another unloadable
			size_t i = 0;
			while (i < objects.size())
			{
				objects[i]->unload();
				i++;
			};
			objects.clear();
			//
		};
	};
	//



	// Shader Object
	struct Shader final : public Loadable, public Unloadable
	{
		uint32_t id = 0;

	
		Shader(std::string files...)
		: files({ files })
		{ };

	protected:
		void load();
		void unload() { };

	private:
		std::vector<std::string> files;
	};
	//



	// Texture Object
	struct Texture final : public Loadable, public Unloadable
	{
		uint32_t id = 0;

		Texture(std::string file, uint32_t type)
		: file(file), type(type)
		{ };

	protected:
		void load();
		void unload() { };

	private:
		std::string file;
		uint32_t type;
	};
	//
};
