#pragma once

#include <glm/glm.hpp>
using namespace glm;

#include <functional>
#include <vector>
#include <string>
#include <mutex>

namespace loom
{
	// Enumeration types
	typedef uint32_t ID;
	typedef uint32_t TYPE;
	//



	// Getting around thread_local bullshit
	struct ShaderManager;
	struct TextureManager;
	ShaderManager* GetSMgr();
	TextureManager* GetTMgr();
	//



	// Manage
	template <typename T>
	struct Manage
	{
	protected:
		friend struct Loom;
		friend struct Camera;
		Manage()
		{
			mut.lock();
			contents.push_back((T*)this);
			mut.unlock();
		};
		virtual ~Manage()
		{
			mut.lock();
			auto it = std::remove(contents.begin(), contents.end(), (T*)this);
			mut.unlock();
		};
		static void access(std::function<void(T*)> f)
		{
			mut.lock();
			for (auto& i : contents)
				f(i);
			mut.unlock();
		};

		static const size_t& size() { return contents.size(); }

	private:
		static inline std::vector<T*> contents;
		static inline std::mutex mut;
	};
	//



	// Object
	// Loads at the start, unloads after
	struct Object : public Manage<Object>
	{
		Object() : Manage<Object>()
		{ };

	protected:
		friend struct Loom;
		virtual void load()=0;
		virtual void unload()=0;
	};
	//



	// Updatable
	// Anything that needs to be updated every frame
	struct Updatable : public Manage<Updatable>
	{
		Updatable() : Manage<Updatable>()
		{ }

	protected:
		friend struct Loom;
		virtual void update()=0;
	};
	//



	// Renderable
	// Anything that needs to be rendered
	struct Renderable : public Manage<Renderable>
	{
		Renderable() : Manage<Renderable>()
		{ };

	protected:
		friend struct Camera;
		virtual void render()=0;
	};
	//



	// Parallel
	// Anything that can run in parallel with the main thread, but needs to be synced
	// When affecting anything outside of this object, use sync
	struct Parallel : public Manage<Parallel>
	{
		Parallel() : Manage<Parallel>()
		{ };

	protected:
		friend struct Loom;
		virtual void sync()=0;
	};
	//


	
	// Shader Object
	struct Shader final : public Object
	{
		Shader(std::string files...)
			: files({ files })
		{ };

		ID id = 0;

	private:
		void load();
		void unload();

		std::vector<std::string> files;
	};
	//



	// Texture Object
	struct Texture final : public Object
	{
		Texture(std::string file, TYPE type)
		: file(file), type(type)
		{ };

		ID id = 0;

	private:
		void load();
		void unload();
		
		std::string file;
		TYPE type;
	};
	//
};
