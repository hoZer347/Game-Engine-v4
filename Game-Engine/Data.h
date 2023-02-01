#pragma once

#include <vector>
#include <string>

namespace loom
{
	// Enumeration types
	typedef uint32_t ID;
	typedef uint32_t TYPE;
	//



	// TODO: Move below to new file
	
	// Getting around thread_local bullshit
	struct ShaderManager;
	struct TextureManager;
	ShaderManager* GetSMgr();
	TextureManager* GetTMgr();
	//



	// Shader Object
	struct Shader final
	{
		ID id = 0;

	//protected:
		friend struct Utils;
		Shader(std::string files...)
		: files({ files })
		{ };

		void load();
		
	private:
		std::vector<std::string> files;
	};
	//



	// Texture Object
	struct Texture final
	{
		ID id = 0;

	//protected:
		friend struct Utils;
		Texture(std::string file, TYPE type)
		: file(file), type(type)
		{ };

		void load();
		
	private:
		std::string file;
		TYPE type;
	};
	//
};
