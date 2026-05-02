#pragma once

#include <glm/glm.hpp>
#include <map>
#include <memory>
#include "chunk.h"

class World {
public:
    World();
    ~World();
    
    void update(const glm::vec3& playerPos);
    void render();
    
    Block getBlock(const glm::ivec3& pos);
    void setBlock(const glm::ivec3& pos, BlockType type);
    
    Chunk* getChunk(const glm::ivec2& chunkPos);
    
private:
    std::map<glm::ivec2, std::shared_ptr<Chunk>> chunks;
    static constexpr int RENDER_DISTANCE = 8;
    
    void loadChunk(const glm::ivec2& chunkPos);
    void unloadChunk(const glm::ivec2& chunkPos);
    glm::ivec2 worldToChunkCoords(const glm::vec3& worldPos);
};
