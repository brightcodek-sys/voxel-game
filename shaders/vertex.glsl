#version 460 core

layout(location = 0) in vec3 position;
layout(location = 1) in vec3 normal;
layout(location = 2) in vec2 texCoord;
layout(location = 3) in vec3 color;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

out VS_OUT {
    vec3 Normal;
    vec2 TexCoord;
    vec3 Color;
    vec3 FragPos;
} vs_out;

void main() {
    gl_Position = projection * view * model * vec4(position, 1.0);
    vs_out.FragPos = vec3(model * vec4(position, 1.0));
    vs_out.Normal = mat3(transpose(inverse(model))) * normal;
    vs_out.TexCoord = texCoord;
    vs_out.Color = color;
}
