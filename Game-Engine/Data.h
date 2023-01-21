#pragma once

#include <glm/glm.hpp>
using namespace glm;

#include <mutex>
#include <queue>
#include <thread>
#include <vector>
#include <string>
#include <iostream>
#include <type_traits>
#include <shared_mutex>

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



	// Object
	struct Object
	{
		virtual void load()=0;
		virtual void unload()=0;

	protected:
		friend struct Loom;
		Object() { objects.push_back(this); };
		static inline std::vector<Object*> objects;
	};
	//



	// Renderable
	struct Renderable : virtual public Object
	{
	protected:
		friend struct Loom;
		virtual void render()=0;
		
	protected:
		friend struct Camera;
		Renderable() { renderables.push_back(this); };
		static inline std::vector<Renderable*> renderables;
	};
	//



	// Updatable
	struct Updatable : virtual public Object
	{
	protected:
		friend struct Loom;
		virtual void update()=0;
		
	protected:
		friend struct Loom;
		Updatable() { updatables.push_back(this); };
		static inline std::vector<Updatable*> updatables;
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
