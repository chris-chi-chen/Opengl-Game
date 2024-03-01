#version 330 core
layout (location = 0) in vec3 aPos; // The vertex position from VBO
layout (location = 1) in vec3 aNormal; // The normal vector from VBO
layout (location = 2) in vec3 aColor; // The color from VBO
layout (location = 3) in vec2 aTexCoords; // The texture coordinate from VBO
out vec3 crntPos;
out vec3 Normal; // Normal to pass to fragment shader
out vec3 Color; // Color to pass to fragment shader
out vec2 TexCoords; // Texture coordinates to pass to fragment shader

uniform mat4 model; // Model matrix
uniform mat4 view; // View matrix
uniform mat4 projection; // Projection matrix
uniform mat4 translation; // Translation matrix
uniform mat4 rotation; // Rotation matrix
uniform mat4 scale; // Scale matrix

void main() {
    crntPos = vec3(model * vec4(aPos, 1.0f));
    gl_Position = projection * view * vec4(crntPos, 1.0);
    Normal = aNormal;
    Color = aColor;
    TexCoords = aTexCoords;
}