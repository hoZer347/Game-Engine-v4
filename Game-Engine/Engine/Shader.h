#pragma once

#include "GLEW/glew.h"
#include "GLFW/glfw3.h"

#include <unordered_map>
#include <string>

namespace loom
{
    struct ShaderManager {
        ShaderManager();
        virtual ~ShaderManager();

        std::unordered_map<std::string, GLuint> shaders;
        std::unordered_map<std::string, GLuint> programs;

        void load_shader(std::string file_name, GLuint shader_type, GLuint program);

        GLuint create(std::vector<std::string> file_names);
        GLuint create(std::string file_name);
    };

    thread_local static inline ShaderManager* s_mgr; // Change to static non-pointer
};
