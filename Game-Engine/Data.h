#pragma once

#include <glm/glm.hpp>
using namespace glm;

#include "Helper.h"

#include <mutex>
#include <queue>
#include <thread>
#include <vector>
#include <string>
#include <iostream>
#include <functional>
#include <shared_mutex>

namespace loom
{
	// Generic Task Object
	typedef std::function<void()> Task;
	//



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



	//
	struct Object
	{
		virtual void load()=0;
		virtual void unload()=0;
	};
	//



	// Renderable
	struct Renderable : public Object
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



	// Shader Object
	struct Shader final
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
	struct Texture final
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
