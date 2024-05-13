#version 440 core

layout(location = 0) in vec4 VColor;
layout(location = 1) in vec4 VPos;
out vec4 FragColor;

void main() {
  FragColor = VPos;
}