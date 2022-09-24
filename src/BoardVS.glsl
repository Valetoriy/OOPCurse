#version 330 core

layout (location = 0) in vec2 aPos;

uniform float x_o;
uniform float y_o;

void main() {
    gl_Position = vec4(aPos.x + x_o, aPos.y + y_o, 0, 1);
}
