#ifndef SHADER_HPP
#define SHADER_HPP

#include <string>

enum Type
{
    SHADER,
    SHADER_PROGRAM
};

class Shader
{
public:    
    Shader(std::string frag_path, std::string vert_path);
    std::string readFromFile(std::string path);
    void checkErrors(Type type, unsigned int id = 0);
    void Use();
    unsigned int getID();

private:
    unsigned int id;
};

#endif
