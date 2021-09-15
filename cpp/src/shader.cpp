#include <shaders/shader.h>

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <fstream>
#include <sstream>
#include <iostream>

Shader::Shader(const char* path) {
    std::string shaderCode;
    std::ifstream shaderFile;

    shaderFile.exceptions (std::ifstream::failbit | std::ifstream::badbit);
    try {
        shaderFile.open(path);
        std::stringstream shaderStream;

        shaderStream << shaderFile.rdbuf();		
        
        shaderFile.close();
        
        shaderCode = shaderStream.str();
    } catch(std::ifstream::failure e) {
        std::cout << "ERROR::SHADER::FILE_NOT_SUCCESFULLY_READ" << std::endl;
    }

    const GLchar* const shaderCodePtr = shaderCode.c_str();

    GLint gComputeProgram = glCreateProgram();
    GLuint mComputeShader = glCreateShader(GL_COMPUTE_SHADER);

    glShaderSource(mComputeShader, 1, &shaderCodePtr, NULL);
    glCompileShader(mComputeShader);

    int rvalue;
    glGetShaderiv(mComputeShader, GL_COMPILE_STATUS, &rvalue);

    if (!rvalue) {

        printf("Error: Compiler log:\n%s\n");

    }

    glAttachShader(gComputeProgram, mComputeShader);
    glLinkProgram(gComputeProgram);

    glGetProgramiv(gComputeProgram, GL_LINK_STATUS, &rvalue);

    if (!rvalue) {
        printf("Error: Linker log:\n%s\n");
    }

}

void Shader::use() { 
    glUseProgram(Shader::ID);
} 

unsigned int Shader::getID() {
    return Shader::ID;
}

void Shader::setBool(const std::string &name, bool value) const {        
    glUniform1i(glGetUniformLocation(Shader::ID, name.c_str()), (int)value); 
}
void Shader::setInt(const std::string &name, int value) const { 
    glUniform1i(glGetUniformLocation(Shader::ID, name.c_str()), value); 
}
void Shader::setFloat(const std::string &name, float value) const { 
    glUniform1f(glGetUniformLocation(Shader::ID, name.c_str()), value); 
} 
void Shader::setVec3(const std::string &name, float value1, float value2, float value3) const { 
    glUniform3f(glGetUniformLocation(Shader::ID, name.c_str()), value1, value2, value3); 
} 
void Shader::setVec4(const std::string &name, float value1, float value2, float value3, float value4) const { 
    glUniform4f(glGetUniformLocation(Shader::ID, name.c_str()), value1, value2, value3, value4); 
} 