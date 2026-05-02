#include "../../include/graphics/renderer.h"
#include <GL/gl.h>
#include <iostream>

Renderer& Renderer::getInstance() {
    static Renderer instance;
    return instance;
}

void Renderer::init(int width, int height) {
    glClearColor(0.5f, 0.7f, 1.0f, 1.0f);
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);
    
    // Create shader (using embedded shader code for now)
    // In production, this would load from files
    shader = new Shader("shaders/vertex.glsl", "shaders/fragment.glsl");
}

void Renderer::beginFrame() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void Renderer::endFrame() {
    // Frame is complete
}

void Renderer::renderMesh(const std::vector<Vertex>& vertices,
                         const std::vector<uint32_t>& indices,
                         const glm::mat4& transform) {
    shader->use();
    shader->setMat4("model", transform);
    shader->setMat4("view", view);
    shader->setMat4("projection", projection);
}

void Renderer::setProjection(const glm::mat4& proj) {
    projection = proj;
}

void Renderer::setView(const glm::mat4& v) {
    view = v;
}
