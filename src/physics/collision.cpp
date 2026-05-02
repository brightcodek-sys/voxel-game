#include "../../include/physics/collision.h"

AABB::AABB(const glm::vec3& center, const glm::vec3& halfSize)
    : min(center - halfSize), max(center + halfSize) {}

bool AABB::intersects(const AABB& other) const {
    return min.x < other.max.x && max.x > other.min.x &&
           min.y < other.max.y && max.y > other.min.y &&
           min.z < other.max.z && max.z > other.min.z;
}

bool AABB::containsPoint(const glm::vec3& point) const {
    return point.x >= min.x && point.x <= max.x &&
           point.y >= min.y && point.y <= max.y &&
           point.z >= min.z && point.z <= max.z;
}

bool Collision::AABBtoAABB(const AABB& a, const AABB& b) {
    return a.intersects(b);
}

bool Collision::rayCast(const glm::vec3& origin, const glm::vec3& direction,
                       const AABB& box, float& distance) {
    float tmin = 0.0f, tmax = 1000.0f;
    
    for (int i = 0; i < 3; ++i) {
        float invD = 1.0f / direction[i];
        float t1 = (box.min[i] - origin[i]) * invD;
        float t2 = (box.max[i] - origin[i]) * invD;
        
        if (invD < 0.0f) std::swap(t1, t2);
        
        tmin = std::max(tmin, t1);
        tmax = std::min(tmax, t2);
        
        if (tmax < tmin) return false;
    }
    
    distance = tmin;
    return true;
}

glm::vec3 Collision::resolveCollision(const AABB& moving, const AABB& fixed) {
    glm::vec3 resolution(0.0f);
    
    // Simple AABB collision resolution
    float overlapLeft = moving.max.x - fixed.min.x;
    float overlapRight = fixed.max.x - moving.min.x;
    float overlapTop = moving.max.y - fixed.min.y;
    float overlapBottom = fixed.max.y - moving.min.y;
    
    // Find the smallest overlap
    float minOverlap = std::min({overlapLeft, overlapRight, overlapTop, overlapBottom});
    
    if (minOverlap == overlapLeft) resolution.x = -overlapLeft;
    else if (minOverlap == overlapRight) resolution.x = overlapRight;
    else if (minOverlap == overlapTop) resolution.y = -overlapTop;
    else if (minOverlap == overlapBottom) resolution.y = overlapBottom;
    
    return resolution;
}
