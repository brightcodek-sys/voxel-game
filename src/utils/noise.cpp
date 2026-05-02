#include "../../include/utils/noise.h"
#include <cmath>
#include <algorithm>

PerlinNoise::PerlinNoise(uint32_t seed) {
    // Initialize permutation table
    for (int i = 0; i < PERMUTATION_SIZE; ++i) {
        permutation[i] = i;
    }
    
    // Shuffle using seed
    for (int i = PERMUTATION_SIZE - 1; i > 0; --i) {
        int j = (seed * 31 + i) % (i + 1);
        std::swap(permutation[i], permutation[j]);
    }
    
    // Duplicate permutation table
    for (int i = 0; i < PERMUTATION_SIZE; ++i) {
        permutation[PERMUTATION_SIZE + i] = permutation[i];
    }
}

float PerlinNoise::noise(float x, float y) const {
    // 2D Perlin noise implementation
    int xi = (int)std::floor(x);
    int yi = (int)std::floor(y);
    
    float xf = x - xi;
    float yf = y - yi;
    
    xi &= 255;
    yi &= 255;
    
    float u = fade(xf);
    float v = fade(yf);
    
    int p00 = permutation[permutation[xi] + yi];
    int p10 = permutation[permutation[xi + 1] + yi];
    int p01 = permutation[permutation[xi] + yi + 1];
    int p11 = permutation[permutation[xi + 1] + yi + 1];
    
    float g00 = grad(p00, xf, yf, 0.0f);
    float g10 = grad(p10, xf - 1.0f, yf, 0.0f);
    float g01 = grad(p01, xf, yf - 1.0f, 0.0f);
    float g11 = grad(p11, xf - 1.0f, yf - 1.0f, 0.0f);
    
    float n0 = lerp(u, g00, g10);
    float n1 = lerp(u, g01, g11);
    
    return lerp(v, n0, n1);
}

float PerlinNoise::noise(float x, float y, float z) const {
    // 3D Perlin noise implementation
    int xi = (int)std::floor(x);
    int yi = (int)std::floor(y);
    int zi = (int)std::floor(z);
    
    float xf = x - xi;
    float yf = y - yi;
    float zf = z - zi;
    
    xi &= 255;
    yi &= 255;
    zi &= 255;
    
    float u = fade(xf);
    float v = fade(yf);
    float w = fade(zf);
    
    int g000 = permutation[permutation[permutation[xi] + yi] + zi];
    int g100 = permutation[permutation[permutation[xi + 1] + yi] + zi];
    int g010 = permutation[permutation[permutation[xi] + yi + 1] + zi];
    int g110 = permutation[permutation[permutation[xi + 1] + yi + 1] + zi];
    int g001 = permutation[permutation[permutation[xi] + yi] + zi + 1];
    int g101 = permutation[permutation[permutation[xi + 1] + yi] + zi + 1];
    int g011 = permutation[permutation[permutation[xi] + yi + 1] + zi + 1];
    int g111 = permutation[permutation[permutation[xi + 1] + yi + 1] + zi + 1];
    
    float n000 = grad(g000, xf, yf, zf);
    float n100 = grad(g100, xf - 1.0f, yf, zf);
    float n010 = grad(g010, xf, yf - 1.0f, zf);
    float n110 = grad(g110, xf - 1.0f, yf - 1.0f, zf);
    float n001 = grad(g001, xf, yf, zf - 1.0f);
    float n101 = grad(g101, xf - 1.0f, yf, zf - 1.0f);
    float n011 = grad(g011, xf, yf - 1.0f, zf - 1.0f);
    float n111 = grad(g111, xf - 1.0f, yf - 1.0f, zf - 1.0f);
    
    float nx00 = lerp(u, n000, n100);
    float nx10 = lerp(u, n010, n110);
    float nx0 = lerp(v, nx00, nx10);
    
    float nx01 = lerp(u, n001, n101);
    float nx11 = lerp(u, n011, n111);
    float nx1 = lerp(v, nx01, nx11);
    
    return lerp(w, nx0, nx1);
}

float PerlinNoise::octaveNoise(float x, float y, int octaves, float persistence, float scale) const {
    float result = 0.0f;
    float amplitude = 1.0f;
    float frequency = 1.0f;
    float maxValue = 0.0f;
    
    for (int i = 0; i < octaves; ++i) {
        result += noise(x * frequency, y * frequency) * amplitude;
        maxValue += amplitude;
        amplitude *= persistence;
        frequency *= scale;
    }
    
    return result / maxValue;
}

float PerlinNoise::fade(float t) const {
    return t * t * t * (t * (t * 6.0f - 15.0f) + 10.0f);
}

float PerlinNoise::lerp(float t, float a, float b) const {
    return a + t * (b - a);
}

float PerlinNoise::grad(int hash, float x, float y, float z) const {
    int h = hash & 15;
    float u = h < 8 ? x : y;
    float v = h < 8 ? y : z;
    return ((h & 1) == 0 ? u : -u) + ((h & 2) == 0 ? v : -v);
}
