#version 460 core

in VS_OUT {
    vec3 Normal;
    vec2 TexCoord;
    vec3 Color;
    vec3 FragPos;
} fs_in;

out vec4 FragColor;

void main() {
    // Simple lighting
    vec3 norm = normalize(fs_in.Normal);
    vec3 lightDir = normalize(vec3(1.0, 1.0, 1.0) - fs_in.FragPos);
    float diff = max(dot(norm, lightDir), 0.2);
    
    vec3 result = fs_in.Color * diff;
    FragColor = vec4(result, 1.0);
}
