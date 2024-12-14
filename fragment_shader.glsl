#version 330 core
out vec4 FragColor;

uniform sampler2D planetTexture; // Текстура планети

in vec2 TexCoords; // Текстурні координати

void main() {
    FragColor = texture(planetTexture, TexCoords); // Отримуємо колір з текстури
}
