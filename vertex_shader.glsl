#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec2 aTexCoord; // Текстурні координати

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

out vec2 TexCoords; // Передаємо текстурні координати у фрагментний шейдер

void main() {
    gl_Position = projection * view * model * vec4(aPos, 1.0);
    TexCoords = aTexCoord; // Передаємо текстурні координати
}
