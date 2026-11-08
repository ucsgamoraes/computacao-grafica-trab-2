#version 330 core
in vec3 ourColor;
in vec2 TexCoord;
out vec4 FragColor;
uniform sampler2D texture1;

void main() {
    FragColor = vec4(texture(texture1, TexCoord).rgb * ourColor, 1.0);
}
