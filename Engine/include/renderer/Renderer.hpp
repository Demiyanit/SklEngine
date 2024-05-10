#pragma once

#include <vector>
#include <string>

#include <renderer/Shader.hpp>
#include <renderer/Mesh.hpp>

#include <glm/glm.hpp>

enum RendererType {
	OpenGL,
	Dx,
	Vulkan
};

class Renderer {
public:
	static void Get(RendererType renderer);
	static void (*Initialize)();
	static void (*Render)();
	static void (*ReRender)();
	static void (*Shutdown)();
	static Shader* (*CreateShader) (std::vector<std::string> paths);
	static void    (*DestroyShader)(Shader* shader);
	static Mesh*   (*CreateMesh)   (std::vector<float> vertices, std::vector<unsigned int> indices);
	static void    (*DestroyMesh)  (Mesh* mesh);
	static void  (*UseShader)  (Shader* shader);
	static void  (*RenderMesh) (Mesh* mesh);
	static void (*ShaderSetUniformBool)(Shader* shader, const std::string& name, bool value);
	static void (*ShaderSetUniformInt)(Shader* shader, const std::string& name, int value);
	static void (*ShaderSetUniformFloat)(Shader* shader, const std::string& name, float value);
	static void (*ShaderSetUniformVec2)(Shader* shader, const std::string& name, const glm::vec2& value);
	static void (*ShaderSetUniformVec3)(Shader* shader, const std::string& name, const glm::vec3& value);
	static void (*ShaderSetUniformVec4)(Shader* shader, const std::string& name, const glm::vec4& value);
	static void (*ShaderSetUniformMat2)(Shader* shader, const std::string& name, const glm::mat2& mat);
	static void (*ShaderSetUniformMat3)(Shader* shader, const std::string& name, const glm::mat3& mat);
	static void (*ShaderSetUniformMat4)(Shader* shader, const std::string& name, const glm::mat4& mat);
	friend class Shader;
	friend class Mesh;
};
