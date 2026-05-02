#pragma once

#include <glm/glm.hpp>

class PerlinNoise {
public:
    PerlinNoise(uint32_t seed = 0);
    
    float noise(float x, float y) const;
    float noise(float x, float y, float z) const;
    
    float octaveNoise(float x, float y, int octaves, float persistence, float scale) const;
    
private:
    static constexpr int PERMUTATION_SIZE = 256;
    int permutation[PERMUTATION_SIZE * 2];
    
    float fade(float t) const;
    float lerp(float t, float a, float b) const;
    float grad(int hash, float x, float y, float z) const;
};
