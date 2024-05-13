#version 440 core

uniform vec4 aColor;
uniform mat4 model;
uniform mat4 projection;
uniform mat4 view;

layout(location = 0) in vec3 vPosition;
layout(location = 0) out vec4 VColor;
layout(location = 1) out vec4 VPos;
void main() {
  gl_Position = projection * view * model * vec4(vPosition, 1.0);
  VColor = aColor;
  VPos = vec4(vPosition, 1.0f);
}