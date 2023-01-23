#pragma once

#include "Manage.h"

#include <glm/glm.hpp>
using namespace glm;

#include <vector>
#include <string>

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



	// Loadable
	// Loads at the start of a frame
	struct Loadable : Manage<Loadable>
	{
	protected:
		friend struct Loom;
		virtual void load()=0;
	};
	//



	// Unloadable
	// Unloads after the runtime is over, or on deletion
	struct Unloadable : Manage<Unloadable>
	{
		// TODO: make sure the main window unloads this

	protected:
		friend struct Loom;
		virtual void unload()=0;
	};
	//



	// Updatable
	// Anything that needs to be updated every frame
	struct Updatable : Manage<Updatable>
	{
	protected:
		friend struct Loom;
		virtual void update()=0;
	};
	//



	// Renderable
	// Anything that needs to be rendered
	struct Renderable : Manage<Renderable>
	{
	protected:
		friend struct Camera;
		virtual void render()=0;
	};
	//



	// Parallel
	// Anything that can run in parallel with the main thread, but needs to be synced
	// When affecting anything outside of this object, use sync
	struct Parallel : Manage<Parallel>
	{
	protected:
		friend struct Loom;
		virtual void sync()=0;
	};
	//


	
	// Shader Object
	struct Shader final : Loadable, Unloadable
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
	//



	// Texture Object
	struct Texture final : Loadable, Unloadable
	{
		Texture(std::string file, TYPE type)
		: file(file), type(type)
		{ };

		ID id = 0;

	private:
		void load() override;
		void unload() override;
		
		std::string file;
		TYPE type;
	};
	//
};
