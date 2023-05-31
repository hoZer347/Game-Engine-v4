#pragma once

#include "Loom.h"
#include "Enums.h"

#include <glm/glm.hpp>
using namespace glm;

#include <mutex>
#include <thread>
#include <atomic>
#include <memory>
#include <vector>
#include <string>
#include <iostream>

namespace loom
{
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
		{ };
		virtual ~GameObject()
		{ };
		static void clear()
		{
			objects.clear();
		};

		static inline std::vector<T*> objects;
	};
	struct Loadable : public GameObject<Loadable>
	{
	protected:
		friend struct Loom;
		virtual void load() = 0;
		static void load_all()
		{
			for (auto i = 0; i < objects.size(); i++)
				objects[i]->load();

			GameObject<Loadable>::clear();
		};
	};
	struct Renderable : public GameObject<Renderable>
	{
	protected:
		friend struct Loom;
		friend struct Camera;
		virtual void render()=0;
		static void render_all()
		{
			for (auto i = 0; i < objects.size(); i++)
				objects[i]->render();
		};
	};
	struct Updateable : public GameObject<Updateable>
	{
	protected:
		friend struct Loom;
		virtual void update()=0;
		static void update_all()
		{
			for (auto i = 0; i < objects.size(); i++)
				objects[i]->update();
		};
	};
	struct Physicsable : public GameObject<Physicsable>
	{
	protected:
		friend struct Loom;
		virtual void physics()=0;
		static void physics_all()
		{
			for (auto i = 0; i < objects.size(); i++)
				objects[i]->physics();
		};
	};
	struct Unloadable : public GameObject<Unloadable>
	{
	protected:
		friend struct Loom;
		virtual void unload()=0;
		static void unload_all()
		{
			for (auto i = 0; i < objects.size(); i++)
				objects[i]->unload();

			GameObject<Unloadable>::clear();
		};
	};
	

	// Shader Object
	struct Shader final :
		virtual protected Loadable,
		virtual protected Unloadable
	{
		// TODO: Make custom class for this, move to new file
		// TODO: Push ShaderManager into this

		uint32_t id = 0;
		
		Shader(std::string files...)
		: files({ files })
		{
			if (!Loom::GetIsRunning())
			{
				GameObject<Loadable>::objects.push_back(this);
				GameObject<Unloadable>::objects.push_back(this);
			};
		};

	private:
		void load();
		void unload() { };

		std::vector<std::string> files;
	};


	// Texture Object
	struct Texture final :
		virtual protected Loadable,
		virtual protected Unloadable
	{
		// TODO: Make custom class for this, move to new file

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
	

	// Allows the object to be treated as an object with geometry
	struct _Geometry
	{
		void operator*=(mat4&& mat) { trns *= mat; };
		void operator=(mat4&& mat) { trns = mat; };

		const uint32_t& collision_type;
		
	protected:
		_Geometry(const uint32_t& collision_type, mat4& trns, std::vector<vec4>& vtxs) :
			collision_type(collision_type),
			trns(trns),
			vtxs(vtxs)
		{ };

	private:
		mat4& trns;
		std::vector<vec4>& vtxs;
		
		// Represents a bounding box
		vec3 pos {0}, size {0};
	};
	template <typename T>
	struct Geometry final : public _Geometry
	{
		Geometry(auto&&... args) :
			obj(args...),
			_Geometry(obj.collision_type, obj.trns, obj.vtxs)
		{ };

		T obj;
	};


	template <typename S>
	void Mount(S* s)
	{
		if constexpr (std::is_base_of_v<Loadable, S>)
			GameObject<Loadable>::objects.push_back((Loadable*)s);
		if constexpr (std::is_base_of_v<Updateable, S>)
			GameObject<Updateable>::objects.push_back((Updateable*)s);
		if constexpr (std::is_base_of_v<Renderable, S>)
			GameObject<Renderable>::objects.push_back((Renderable*)s);
		if constexpr (std::is_base_of_v<Physicsable, S>)
			GameObject<Physicsable>::objects.push_back((Physicsable*)s);
		if constexpr (std::is_base_of_v<Unloadable, S>)
			GameObject<Unloadable>::objects.push_back((Unloadable*)s);
	};
	template <typename S, typename T, typename... REMAINING>
	void Mount(S* s, T* t, REMAINING*... to_add)
	{
		Mount<S>(s);
		Mount<T, REMAINING...>(t, to_add...);
	};
	

	template <typename S>
	void Unmount(S* s)
	{
		std::cout << "UNMOUNT NOT SUPPORTED YET" << std::endl;
	};
	template <typename S, typename T, typename... REMAINING>
	void Unmount(S* s, T* t, REMAINING*... to_add)
	{
		Unmount<S>(s);
		Unmount<T, REMAINING...>(t, to_add...);
	};
};
