#pragma once

#include <glm/glm.hpp>
#include <vector>
#include "block.h"
#include "../graphics/renderer.h"

class Chunk {
public:
    static constexpr int WIDTH = 16;
    static constexpr int HEIGHT = 256;
    static constexpr int DEPTH = 16;
    
    Chunk(const glm::ivec2& chunkPos);
    ~Chunk();
    
    void generateTerrain();
    void buildMesh();
    void render();
    
    Block getBlock(const glm::ivec3& localPos);
    void setBlock(const glm::ivec3& localPos, BlockType type);
    
    bool isBlockOccluded(const glm::ivec3& pos);
    glm::ivec2 getChunkPos() const { return chunkPos; }
    
private:
    glm::ivec2 chunkPos;
    Block blocks[WIDTH][HEIGHT][DEPTH];
    std::vector<Vertex> vertices;
    std::vector<uint32_t> indices;
    uint32_t VAO, VBO, EBO;
    bool meshDirty;
    
    void addFace(const glm::ivec3& pos, Direction dir);
};
