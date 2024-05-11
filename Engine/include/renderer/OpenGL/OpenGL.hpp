#pragma once

#include <renderer/Mesh.hpp>
#include <renderer/Shader.hpp>
#include <glm/glm.hpp>
void OGLInit();
void OGLRender(glm::mat4 matrix, Shader* shader, Mesh* mesh);
void OGLReRender();
void OGLShutDown();
Shader CreateOGLShader(std::vector<std::string> paths);
Mesh  CreateOGLMesh(std::vector<float> vertices, std::vector<unsigned int> indices);
void   DestroyOGLShader(Shader* shader);
void   DestroyOGLMesh(Mesh* mesh);
void UseOGLShader(Shader* shader);
void RenderOGLMesh(Mesh* mesh);
void OGLShaderSetUniformBool(Shader* shader, const std::string& name, bool value);
void OGLShaderSetUniformInt(Shader* shader, const std::string& name, int value);
void OGLShaderSetUniformFloat(Shader* shader, const std::string& name, float value);
void OGLShaderSetUniformVec2(Shader* shader, const std::string& name, const glm::vec2& value);
void OGLShaderSetUniformVec3(Shader* shader, const std::string& name, const glm::vec3& value);
void OGLShaderSetUniformVec4(Shader* shader, const std::string& name, const glm::vec4& value);
void OGLShaderSetUniformMat2(Shader* shader, const std::string& name, const glm::mat2& mat);
void OGLShaderSetUniformMat3(Shader* shader, const std::string& name, const glm::mat3& mat);
void OGLShaderSetUniformMat4(Shader* shader, const std::string& name, const glm::mat4& mat);