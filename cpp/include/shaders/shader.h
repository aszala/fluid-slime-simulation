#ifndef SHADER_H
#define SHADER_H
  
#include <string>
  

class Shader {
public:
    unsigned int ID;
  
    Shader(const char* path);
    
    void use();
    
    unsigned int getID();
    void setBool(const std::string &name, bool value) const;  
    void setInt(const std::string &name, int value) const;   
    void setFloat(const std::string &name, float value) const;
    void setVec3(const std::string &name, float value1, float value2, float value3) const;
    void setVec4(const std::string &name, float value1, float value2, float value3, float value4) const;
};
  
#endif