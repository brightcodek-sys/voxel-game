#pragma once

#include <glm/glm.hpp>

struct AABB {
    glm::vec3 min;
    glm::vec3 max;
    
    AABB(const glm::vec3& center, const glm::vec3& halfSize);
    
    bool intersects(const AABB& other) const;
    bool containsPoint(const glm::vec3& point) const;
};

class Collision {
public:
    static bool AABBtoAABB(const AABB& a, const AABB& b);
    static bool rayCast(const glm::vec3& origin, const glm::vec3& direction, 
                       const AABB& box, float& distance);
    static glm::vec3 resolveCollision(const AABB& moving, const AABB& fixed);
};
