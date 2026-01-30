#ifndef _SHADER_H_
#define _SHADER_H_

#include <glad/glad.h>
#include <helper.h>
#include <filesystem>
#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <cerrno>
#include <cstring>

std::string get_file_contents(const char* filename);

class Shader{
    public: 
        GLuint ID;
        Shader(const char* vertexFile, const char* fragmentFile);

        void use();
        void terminate();

        void setBool(const std::string &name, bool value) const;  
        void setInt(const std::string &name, int value) const;   
        void setFloat(const std::string &name, float value) const;
        void setVec3(const std::string &name, float x, float y, float z) const;

    protected:
        void compileErrors(unsigned int shader, const char* type);
};

#endif /*_SHADER_H_*/