#pragma once

#include <glm/glm.hpp>
#include <string>
#include <cstdint>

class Shader {
public:
    Shader(const std::string& vertexPath, const std::string& fragmentPath);
    ~Shader();
    
    void use();
    void setMat4(const std::string& name, const glm::mat4& mat);
    void setVec3(const std::string& name, const glm::vec3& vec);
    void setFloat(const std::string& name, float value);
    void setInt(const std::string& name, int value);
    
private:
    uint32_t program;
    
    uint32_t compileShader(const std::string& source, uint32_t type);
    std::string readShaderFile(const std::string& path);
};
