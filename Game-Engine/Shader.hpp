#include "Mem_Leak.h"

#include "GLEW/glew.h"
#include "GLFW/glfw3.h"

#include <unordered_map>
#include <fstream>
#include <string>

#include <iostream>

namespace eng {
    class ShaderManager {
        static inline std::unordered_map<std::string, GLint> shaders;
        static inline std::unordered_map<std::string, GLint> programs;

        static void load_shader(
            std::string file_name,
            GLint shader_type,
            GLint program) {
            if (shaders[file_name]) {
                glAttachShader(program, shaders[file_name]);
                return;
            };

            std::ifstream f(file_name.c_str());

            if (f.good()) {
                GLint shader = glCreateShader(shader_type);

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
        static unsigned int create(const char* file_name) {
            std::string f = file_name;

            if (programs[f])
                return programs[f];

            unsigned int program = glCreateProgram();

            load_shader(f + ".vert", GL_VERTEX_SHADER, program);
            load_shader(f + ".geom", GL_GEOMETRY_SHADER, program);
            load_shader(f + ".frag", GL_FRAGMENT_SHADER, program);

            glLinkProgram(program);

            shaders[f] = program;

            return program;
        };

        static unsigned int create(
            const char* vs_file_name,
            const char* fs_file_name) {
            std::string f;
            f += vs_file_name;
            f += fs_file_name;

            if (programs[f])
                return programs[f];

            unsigned int program = glCreateProgram();

            load_shader(vs_file_name, GL_VERTEX_SHADER, program);
            load_shader(fs_file_name, GL_FRAGMENT_SHADER, program);

            glLinkProgram(program);

            shaders[f] = program;

            return program;
        };

        static unsigned int create(
            const char* vs_file_name,
            const char* gs_file_name,
            const char* fs_file_name) {
            std::string f;

            f += vs_file_name;
            f += gs_file_name;
            f += fs_file_name;

            if (programs[f])
                return programs[f];

            unsigned int program = glCreateProgram();

            load_shader(vs_file_name, GL_VERTEX_SHADER, program);
            load_shader(gs_file_name, GL_GEOMETRY_SHADER, program);
            load_shader(fs_file_name, GL_FRAGMENT_SHADER, program);

            glLinkProgram(program);

            shaders[f] = program;

            return program;
        };

        static void resources() {
            std::cout << shaders.size() << std::endl;
            std::cout << programs.size() << std::endl;
        };
    };
};
