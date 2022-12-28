#pragma once

#include "Data.h"

#include "GLEW/glew.h"
#include "GLFW/glfw3.h"

#include <unordered_map>
#include <fstream>
#include <string>

#include <iostream>

namespace loom
{
    typedef ID SubShader;
    typedef ID ShaderType;

    struct ShaderManager {
        std::unordered_map<std::string, SubShader> shaders;
        std::unordered_map<std::string, Shader> programs;

    private:
        void load_shader(
            std::string file_name,
            ShaderType shader_type,
            Shader program) {
            if (shaders[file_name]) {
                glAttachShader(program, shaders[file_name]);
                return;
            };

            std::ifstream f(file_name.c_str());

            if (f.good()) {
                SubShader shader = glCreateShader(shader_type);

                std::string _fs((std::istreambuf_iterator<char>(f)),
                    std::istreambuf_iterator<char>());
                const char* _shader = _fs.c_str();

                glShaderSource(shader, 1, &_shader, NULL);
                glCompileShader(shader);
                glAttachShader(program, shader);

                shaders[file_name] = shader;
            };
        };

    public:
        Shader create(std::vector<std::string> file_names)
        {
            if (file_names.size() == 1)
                return create(file_names.back());

            std::string f;

            for (auto& file : file_names)
                f += file;

            if (programs[f])
                return programs[f];

            Shader program = glCreateProgram();

            for (auto& file : file_names)
            {
                     if (file.find(".vert") != std::string::npos) load_shader(file, GL_VERTEX_SHADER, program);
                else if (file.find(".geom") != std::string::npos) load_shader(file, GL_GEOMETRY_SHADER, program);
                else if (file.find(".frag") != std::string::npos) load_shader(file, GL_FRAGMENT_SHADER, program);
                else if (file.find(".comp") != std::string::npos) load_shader(file, GL_COMPUTE_SHADER, program);
                else if (file.find(".tess") != std::string::npos) load_shader(file, GL_TESS_CONTROL_SHADER, program);
                else if (file.find(".eval") != std::string::npos) load_shader(file, GL_TESS_EVALUATION_SHADER, program);
                else std::cout << "Couldn't find " << file << std::endl;
            };

            glLinkProgram(program);

            return programs[f] = program;
        };

        Shader create(std::string file_name)
        {
            std::string f = file_name;

            if (programs[f])
                return programs[f];

            Shader program = glCreateProgram();

            load_shader(f + ".vert", GL_VERTEX_SHADER, program);
            load_shader(f + ".geom", GL_GEOMETRY_SHADER, program);
            load_shader(f + ".frag", GL_FRAGMENT_SHADER, program);
            load_shader(f + ".comp", GL_COMPUTE_SHADER, program);
            load_shader(f + ".tess", GL_TESS_CONTROL_SHADER, program);
            load_shader(f + ".eval", GL_TESS_EVALUATION_SHADER, program);

            glLinkProgram(program);
            
            return shaders[f] = program;
        };
    };
};
