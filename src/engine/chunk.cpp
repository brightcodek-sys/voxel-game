#include "../../include/engine/chunk.h"
#include "../../include/utils/noise.h"
#include <glm/gtc/matrix_transform.hpp>
#include <iostream>

Chunk::Chunk(const glm::ivec2& chunkPos) 
    : chunkPos(chunkPos), VAO(0), VBO(0), EBO(0), meshDirty(true) {
    
    // Initialize all blocks to air
    for (int x = 0; x < WIDTH; ++x) {
        for (int y = 0; y < HEIGHT; ++y) {
            for (int z = 0; z < DEPTH; ++z) {
                blocks[x][y][z] = Block(BlockType::AIR);
            }
        }
    }
}

Chunk::~Chunk() {
    if (VAO) glDeleteVertexArrays(1, &VAO);
    if (VBO) glDeleteBuffers(1, &VBO);
    if (EBO) glDeleteBuffers(1, &EBO);
}

void Chunk::generateTerrain() {
    PerlinNoise noise(chunkPos.x * 73856093 ^ chunkPos.y * 19349663);
    
    for (int x = 0; x < WIDTH; ++x) {
        for (int z = 0; z < DEPTH; ++z) {
            float worldX = chunkPos.x * WIDTH + x;
            float worldZ = chunkPos.y * DEPTH + z;
            
            // Generate height using Perlin noise
            float height = noise.octaveNoise(worldX * 0.01f, worldZ * 0.01f, 4, 0.5f, 2.0f);
            int terrainHeight = static_cast<int>((height + 1.0f) * 0.5f * 64.0f) + 32;
            
            for (int y = 0; y < HEIGHT; ++y) {
                if (y < terrainHeight - 5) {
                    blocks[x][y][z] = Block(BlockType::STONE);
                } else if (y < terrainHeight) {
                    blocks[x][y][z] = Block(BlockType::DIRT);
                } else if (y == terrainHeight) {
                    blocks[x][y][z] = Block(BlockType::GRASS);
                } else if (y < terrainHeight + 3) {
                    blocks[x][y][z] = Block(BlockType::AIR);
                }
            }
        }
    }
}

void Chunk::buildMesh() {
    vertices.clear();
    indices.clear();
    
    for (int x = 0; x < WIDTH; ++x) {
        for (int y = 0; y < HEIGHT; ++y) {
            for (int z = 0; z < DEPTH; ++z) {
                if (blocks[x][y][z].isSolid()) {
                    glm::ivec3 pos(x, y, z);
                    
                    // Check each face
                    if (y + 1 >= HEIGHT || !blocks[x][y+1][z].isSolid()) 
                        addFace(pos, Direction::TOP);
                    if (y == 0 || !blocks[x][y-1][z].isSolid()) 
                        addFace(pos, Direction::BOTTOM);
                    if (z + 1 >= DEPTH || !blocks[x][y][z+1].isSolid()) 
                        addFace(pos, Direction::FRONT);
                    if (z == 0 || !blocks[x][y][z-1].isSolid()) 
                        addFace(pos, Direction::BACK);
                    if (x == 0 || !blocks[x-1][y][z].isSolid()) 
                        addFace(pos, Direction::LEFT);
                    if (x + 1 >= WIDTH || !blocks[x+1][y][z].isSolid()) 
                        addFace(pos, Direction::RIGHT);
                }
            }
        }
    }
    
    // Create VAO, VBO, EBO
    if (VAO == 0) {
        glGenVertexArrays(1, &VAO);
        glGenBuffers(1, &VBO);
        glGenBuffers(1, &EBO);
    }
    
    glBindVertexArray(VAO);
    
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), vertices.data(), GL_DYNAMIC_DRAW);
    
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(uint32_t), indices.data(), GL_DYNAMIC_DRAW);
    
    // Vertex attributes
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0);
    
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, normal));
    
    glEnableVertexAttribArray(2);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, texCoord));
    
    glEnableVertexAttribArray(3);
    glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, color));
    
    glBindVertexArray(0);
    meshDirty = false;
}

void Chunk::render() {
    if (indices.empty()) return;
    
    glBindVertexArray(VAO);
    glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);
}

Block Chunk::getBlock(const glm::ivec3& localPos) {
    if (localPos.x < 0 || localPos.x >= WIDTH ||
        localPos.y < 0 || localPos.y >= HEIGHT ||
        localPos.z < 0 || localPos.z >= DEPTH) {
        return Block(BlockType::AIR);
    }
    return blocks[localPos.x][localPos.y][localPos.z];
}

void Chunk::setBlock(const glm::ivec3& localPos, BlockType type) {
    if (localPos.x < 0 || localPos.x >= WIDTH ||
        localPos.y < 0 || localPos.y >= HEIGHT ||
        localPos.z < 0 || localPos.z >= DEPTH) {
        return;
    }
    blocks[localPos.x][localPos.y][localPos.z] = Block(type);
    meshDirty = true;
}

void Chunk::addFace(const glm::ivec3& pos, Direction dir) {
    uint32_t baseIndex = vertices.size();
    glm::vec3 color(0.8f, 0.8f, 0.8f);
    
    // Simplified cube face generation
    switch (dir) {
        case Direction::TOP:
            vertices.push_back({{pos.x, pos.y + 1, pos.z}, {0, 1, 0}, {0, 0}, color});
            vertices.push_back({{pos.x + 1, pos.y + 1, pos.z}, {0, 1, 0}, {1, 0}, color});
            vertices.push_back({{pos.x + 1, pos.y + 1, pos.z + 1}, {0, 1, 0}, {1, 1}, color});
            vertices.push_back({{pos.x, pos.y + 1, pos.z + 1}, {0, 1, 0}, {0, 1}, color});
            break;
        case Direction::BOTTOM:
            vertices.push_back({{pos.x, pos.y, pos.z}, {0, -1, 0}, {0, 0}, color});
            vertices.push_back({{pos.x, pos.y, pos.z + 1}, {0, -1, 0}, {0, 1}, color});
            vertices.push_back({{pos.x + 1, pos.y, pos.z + 1}, {0, -1, 0}, {1, 1}, color});
            vertices.push_back({{pos.x + 1, pos.y, pos.z}, {0, -1, 0}, {1, 0}, color});
            break;
        case Direction::FRONT:
            vertices.push_back({{pos.x, pos.y, pos.z + 1}, {0, 0, 1}, {0, 0}, color});
            vertices.push_back({{pos.x + 1, pos.y, pos.z + 1}, {0, 0, 1}, {1, 0}, color});
            vertices.push_back({{pos.x + 1, pos.y + 1, pos.z + 1}, {0, 0, 1}, {1, 1}, color});
            vertices.push_back({{pos.x, pos.y + 1, pos.z + 1}, {0, 0, 1}, {0, 1}, color});
            break;
        case Direction::BACK:
            vertices.push_back({{pos.x + 1, pos.y, pos.z}, {0, 0, -1}, {0, 0}, color});
            vertices.push_back({{pos.x, pos.y, pos.z}, {0, 0, -1}, {1, 0}, color});
            vertices.push_back({{pos.x, pos.y + 1, pos.z}, {0, 0, -1}, {1, 1}, color});
            vertices.push_back({{pos.x + 1, pos.y + 1, pos.z}, {0, 0, -1}, {0, 1}, color});
            break;
        case Direction::LEFT:
            vertices.push_back({{pos.x, pos.y, pos.z}, {-1, 0, 0}, {0, 0}, color});
            vertices.push_back({{pos.x, pos.y, pos.z + 1}, {-1, 0, 0}, {1, 0}, color});
            vertices.push_back({{pos.x, pos.y + 1, pos.z + 1}, {-1, 0, 0}, {1, 1}, color});
            vertices.push_back({{pos.x, pos.y + 1, pos.z}, {-1, 0, 0}, {0, 1}, color});
            break;
        case Direction::RIGHT:
            vertices.push_back({{pos.x + 1, pos.y, pos.z + 1}, {1, 0, 0}, {0, 0}, color});
            vertices.push_back({{pos.x + 1, pos.y, pos.z}, {1, 0, 0}, {1, 0}, color});
            vertices.push_back({{pos.x + 1, pos.y + 1, pos.z}, {1, 0, 0}, {1, 1}, color});
            vertices.push_back({{pos.x + 1, pos.y + 1, pos.z + 1}, {1, 0, 0}, {0, 1}, color});
            break;
    }
    
    // Add indices for the face (two triangles)
    indices.push_back(baseIndex);
    indices.push_back(baseIndex + 1);
    indices.push_back(baseIndex + 2);
    indices.push_back(baseIndex);
    indices.push_back(baseIndex + 2);
    indices.push_back(baseIndex + 3);
}
