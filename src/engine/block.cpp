#include "../../include/engine/block.h"

bool Block::isSolid() const {
    return type != BlockType::AIR && type != BlockType::WATER;
}

bool Block::isTransparent() const {
    return type == BlockType::AIR || type == BlockType::WATER || type == BlockType::LEAVES;
}

glm::vec2 Block::getTexCoord(Direction dir) const {
    // Simple texture coordinates for now
    // In a full implementation, these would map to a texture atlas
    return glm::vec2(0.0f, 0.0f);
}
