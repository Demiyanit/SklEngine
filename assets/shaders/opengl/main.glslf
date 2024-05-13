#version 440 core

layout(location = 0) in vec4 VColor;
out vec4 FragColor;

void main() {
  FragColor = VColor;
}