#pragma once

#include <glm/glm.hpp>
#include <vector>
#include <cstdint>
#include "shader.h"

struct Vertex {
    glm::vec3 position;
    glm::vec3 normal;
    glm::vec2 texCoord;
    glm::vec3 color;
};

class Renderer {
public:
    static Renderer& getInstance();
    
    void init(int width, int height);
    void beginFrame();
    void endFrame();
    
    void renderMesh(const std::vector<Vertex>& vertices, 
                   const std::vector<uint32_t>& indices,
                   const glm::mat4& transform);
    
    void setProjection(const glm::mat4& proj);
    void setView(const glm::mat4& view);
    
private:
    Renderer() = default;
    
    Shader* shader;
    glm::mat4 projection;
    glm::mat4 view;
};
