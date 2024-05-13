#pragma once

#include <vector>
#include <string>

#include <renderer/Shader.hpp>
#include <renderer/Mesh.hpp>
#include <renderer/Texture.hpp>
#include <glm/glm.hpp>

class RenderData {
public:
	Shader main_shader;
	Mesh object_mesh;
	glm::mat4 object_matrix;
	glm::vec4 color = glm::vec4(0.0f);
};

enum RendererType {
	OpenGL,
	Dx,
	Vulkan
};

class Renderer {
public:
	static void Get(RendererType renderer);
	static void (*Initialize)();
	static void (*SetViewPort)(int x, int y , int width, int height);
	static void (*ClearColor)(glm::vec4 color);
	static void (*StartRender)();
	static void (*Render)(glm::mat4 projection, glm::mat4 view, RenderData data);
	static void (*FinishRender)();
	static Shader  (*CreateShader) (std::vector<std::string> paths);
	static void    (*DestroyShader)(Shader* shader);
	static Mesh    (*CreateMesh)   (std::vector<float> vertices, std::vector<unsigned int> indices);
	static void    (*DestroyMesh)  (Mesh* mesh);
	
	static glm::mat4 proj;
};
