#pragma once

#include <renderer/Renderer.hpp>
#include <glm/glm.hpp>
void OGLInit();
void OGLStartRender();
void OGLRender(glm::mat4 projection, glm::mat4 view, RenderData data);
void OGLFinishRender();
Shader  CreateOGLShader(std::vector<std::string> paths);
Mesh    CreateOGLMesh(std::vector<float> vertices, std::vector<unsigned int> indices);
Texture CreateOGLTexture(std::string path);
void   DestroyOGLShader(Shader* shader);
void   DestroyOGLMesh(Mesh* mesh);
void   DestroyOGLTexture(Texture* texture);
void UseOGLShader(Shader* shader);
void RenderOGLMesh(Mesh* mesh);
void OGLSetViewport(int x, int y, int width, int height);
void OGLClearColor(glm::vec4 color);
void OGLShaderSetUniformBool(Shader* shader, const std::string& name, bool value);
void OGLShaderSetUniformInt(Shader* shader, const std::string& name, int value);
void OGLShaderSetUniformFloat(Shader* shader, const std::string& name, float value);
void OGLShaderSetUniformVec2(Shader* shader, const std::string& name, const glm::vec2& value);
void OGLShaderSetUniformVec3(Shader* shader, const std::string& name, const glm::vec3& value);
void OGLShaderSetUniformVec4(Shader* shader, const std::string& name, const glm::vec4& value);
void OGLShaderSetUniformMat2(Shader* shader, const std::string& name, const glm::mat2& mat);
void OGLShaderSetUniformMat3(Shader* shader, const std::string& name, const glm::mat3& mat);
void OGLShaderSetUniformMat4(Shader* shader, const std::string& name, const glm::mat4& mat);