#pragma once

#include <glm/glm.hpp>
#include <cstdint>

enum class BlockType : uint8_t {
    AIR = 0,
    GRASS = 1,
    DIRT = 2,
    STONE = 3,
    WOOD = 4,
    LEAVES = 5,
    SAND = 6,
    WATER = 7,
    BEDROCK = 8
};

enum class Direction {
    TOP = 0,
    BOTTOM = 1,
    FRONT = 2,
    BACK = 3,
    LEFT = 4,
    RIGHT = 5
};

struct Block {
    BlockType type;
    
    Block(BlockType t = BlockType::AIR) : type(t) {}
    
    bool isSolid() const;
    bool isTransparent() const;
    glm::vec2 getTexCoord(Direction dir) const;
};
