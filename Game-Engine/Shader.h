#pragma once

#include "Data.h"

#include "GLEW/glew.h"
#include "GLFW/glfw3.h"

#include <unordered_map>

#include <fstream>
#include <iostream>

namespace loom
{
	struct ShaderManager {
		virtual ~ShaderManager();

		std::unordered_map<std::string, GLuint> shaders;
		std::unordered_map<std::string, GLuint> programs;

		void load_shader(std::string file_name, GLuint shader_type, GLuint program);

		GLuint create(std::vector<std::string> file_names);
		GLuint create(std::string file_name);
	};
	static inline std::shared_ptr<ShaderManager> s_mgr = std::make_shared<ShaderManager>();


	// Shader Object
	struct Shader final :
		public GameObject<Shader>
	{
		uint32_t id = 0;

	protected:
		friend struct ptr<Shader>;
		Shader(auto&&... files) :
			files({ files... })
		{
			Engine::DoOnMain([=]()
			{
				id = s_mgr->create(files...);
			});
		};

	private:
		std::vector<std::string> files;
	};


	inline ShaderManager::~ShaderManager()
	{
		for (auto& i : shaders)
			glDeleteShader(i.second);

		for (auto& i : programs)
			glDeleteProgram(i.second);
	};
	inline void ShaderManager::load_shader(std::string file_name, GLuint shader_type, GLuint program)
	{
		if (shaders[file_name]) {
			glAttachShader(program, shaders[file_name]);
			return;
		};

		std::ifstream f(file_name.c_str());

		if (f.good()) {
			GLuint shader = glCreateShader(shader_type);

			std::string _fs((std::istreambuf_iterator<char>(f)),
				std::istreambuf_iterator<char>());
			const char* _shader = _fs.c_str();

			glShaderSource(shader, 1, &_shader, NULL);
			glCompileShader(shader);
			glAttachShader(program, shader);

			shaders[file_name] = shader;
		};
	};
	inline GLuint ShaderManager::create(std::vector<std::string> file_names)
	{
		if (file_names.size() == 1)
			return create(file_names.back());

		std::string f;

		for (auto& file : file_names)
			f += file;

		//if (programs[f])
		//    return programs[f];

		GLuint program = glCreateProgram();

		for (auto& file : file_names)
		{
			if (file.find(".vert") != std::string::npos) load_shader(file, GL_VERTEX_SHADER, program);
			else if (file.find(".geom") != std::string::npos) load_shader(file, GL_GEOMETRY_SHADER, program);
			else if (file.find(".frag") != std::string::npos) load_shader(file, GL_FRAGMENT_SHADER, program);
			else if (file.find(".comp") != std::string::npos) load_shader(file, GL_COMPUTE_SHADER, program);
			else if (file.find(".tess") != std::string::npos) load_shader(file, GL_TESS_CONTROL_SHADER, program);
			else if (file.find(".eval") != std::string::npos) load_shader(file, GL_TESS_EVALUATION_SHADER, program);
			else std::cerr << "Couldn't find " << file << std::endl;
		};

		glLinkProgram(program);

		glGetError();

		return programs[f] = program;
	};
	inline GLuint ShaderManager::create(std::string file_name)
	{
		if (programs[file_name])
			return programs[file_name];

		GLuint program = glCreateProgram();

		load_shader(file_name + ".vert", GL_VERTEX_SHADER, program);
		load_shader(file_name + ".geom", GL_GEOMETRY_SHADER, program);
		load_shader(file_name + ".frag", GL_FRAGMENT_SHADER, program);
		load_shader(file_name + ".comp", GL_COMPUTE_SHADER, program);
		load_shader(file_name + ".tess", GL_TESS_CONTROL_SHADER, program);
		load_shader(file_name + ".eval", GL_TESS_EVALUATION_SHADER, program);

		glLinkProgram(program);

		glGetError();

		return shaders[file_name] = program;
	};
};
