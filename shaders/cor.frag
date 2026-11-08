#version 330 core
in vec3 ourColor;
out vec4 FragColor;
uniform vec3 tint;
uniform bool selected;

void main() {
    vec3 color = ourColor * tint;
    if (selected) color = mix(color, vec3(1.0, 0.63, 0.08), 0.55);
    FragColor = vec4(color, 1.0);
}
