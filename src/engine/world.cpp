#include "../../include/engine/world.h"
#include "../../include/utils/noise.h"
#include <glm/glm.hpp>

World::World() {}

World::~World() {
    chunks.clear();
}

void World::update(const glm::vec3& playerPos) {
    glm::ivec2 chunkPos = worldToChunkCoords(playerPos);
    
    // Load chunks around player
    for (int x = chunkPos.x - RENDER_DISTANCE; x <= chunkPos.x + RENDER_DISTANCE; ++x) {
        for (int z = chunkPos.y - RENDER_DISTANCE; z <= chunkPos.y + RENDER_DISTANCE; ++z) {
            glm::ivec2 pos(x, z);
            if (chunks.find(pos) == chunks.end()) {
                loadChunk(pos);
            }
        }
    }
    
    // Unload distant chunks
    auto it = chunks.begin();
    while (it != chunks.end()) {
        int dx = it->first.x - chunkPos.x;
        int dz = it->first.y - chunkPos.y;
        if (dx * dx + dz * dz > RENDER_DISTANCE * RENDER_DISTANCE * 2) {
            it = chunks.erase(it);
        } else {
            ++it;
        }
    }
}

void World::render() {
    for (auto& pair : chunks) {
        pair.second->render();
    }
}

Block World::getBlock(const glm::ivec3& pos) {
    glm::ivec2 chunkPos = glm::ivec2(pos.x / 16, pos.z / 16);
    auto it = chunks.find(chunkPos);
    if (it != chunks.end()) {
        glm::ivec3 localPos(
            ((pos.x % 16) + 16) % 16,
            pos.y,
            ((pos.z % 16) + 16) % 16
        );
        return it->second->getBlock(localPos);
    }
    return Block(BlockType::AIR);
}

void World::setBlock(const glm::ivec3& pos, BlockType type) {
    glm::ivec2 chunkPos = glm::ivec2(pos.x / 16, pos.z / 16);
    auto it = chunks.find(chunkPos);
    if (it != chunks.end()) {
        glm::ivec3 localPos(
            ((pos.x % 16) + 16) % 16,
            pos.y,
            ((pos.z % 16) + 16) % 16
        );
        it->second->setBlock(localPos, type);
        it->second->buildMesh();
    }
}

Chunk* World::getChunk(const glm::ivec2& chunkPos) {
    auto it = chunks.find(chunkPos);
    return (it != chunks.end()) ? it->second.get() : nullptr;
}

void World::loadChunk(const glm::ivec2& chunkPos) {
    auto chunk = std::make_shared<Chunk>(chunkPos);
    chunk->generateTerrain();
    chunk->buildMesh();
    chunks[chunkPos] = chunk;
}

void World::unloadChunk(const glm::ivec2& chunkPos) {
    chunks.erase(chunkPos);
}

glm::ivec2 World::worldToChunkCoords(const glm::vec3& worldPos) {
    return glm::ivec2(
        static_cast<int>(worldPos.x) / 16,
        static_cast<int>(worldPos.z) / 16
    );
}
