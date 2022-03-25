#include "shader.hpp"
#include <fstream>
#include <iostream>
#include <glad/glad.h>

Shader::Shader(std::string frag_path, std::string vert_path)
{
    std::string frag_src = readFromFile(frag_path);
    std::string vert_src = readFromFile(vert_path);

    const char * c_frag_src = frag_src.c_str();
    const char * c_vert_src = vert_src.c_str();

    unsigned int frag_shader;
    frag_shader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(frag_shader, 1, &c_frag_src, nullptr);
    glCompileShader(frag_shader);
    checkErrors(SHADER, frag_shader);
    
    unsigned int vert_shader;
    vert_shader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vert_shader, 1, &c_vert_src, nullptr);
    glCompileShader(vert_shader);
    checkErrors(SHADER, vert_shader);

    id = glCreateProgram();
    glAttachShader(id, frag_shader);
    glAttachShader(id, vert_shader);
    glLinkProgram(id);
    checkErrors(SHADER_PROGRAM);

    glDeleteShader(frag_shader);
    glDeleteShader(vert_shader);
}

std::string Shader::readFromFile(std::string path)
{
    std::ifstream is(path);
    std::string buffer = "\n";

    if (is) getline(is, buffer, '\0');
    else std::cout << "Error: can't find " << path << "\n";
    
    is.close();
    return buffer;
}

void Shader::checkErrors(Type type, unsigned int id)
{
    int success;
    char logs[512];

    if (type == SHADER) glGetShaderiv(id, GL_COMPILE_STATUS, &success);
    else glGetProgramiv(this->id, GL_LINK_STATUS, &success);

    if(!success)
    {
        if (type == SHADER)
        {
            glGetShaderInfoLog(id, 512, nullptr, logs);
            std::cout << "Shader compilation error:\n" << logs << "\n";
        }
        else
        {
            glGetProgramInfoLog(id, 512, nullptr, logs);
            std::cout << "Shader program link error:\n" << logs << "\n";
        }
    }
}

void Shader::use() { glUseProgram(id); }

unsigned int Shader::getID() { return this->id; }
