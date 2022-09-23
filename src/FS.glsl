#version 330 core

out vec4 Color;

in vec2 TexCoord;

uniform sampler2D tex;

void main() {
    Color = texture(tex, TexCoord);
}
