#version 330 core

in vec4 vColor;
in vec2 vUvPosition;

uniform sampler2D uFontTexture;

out vec4 FragColor;

void main() {
    FragColor = vec4(vColor.x, vColor.y, vColor.z, vColor.w) * texture(uFontTexture, vUvPosition).r;
}
