#include <core/EngineEvents.hpp>
#include <renderer/OpenGL/OpenGL.hpp>
#include <renderer/Renderer.hpp>
#include <renderer/Shader.hpp>
#include <renderer/Mesh.hpp>
#include <platform/Window.hpp>
#include <stdexcept>
#include <fstream>
#include <sstream>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glew/glew.h>
#include <iostream>

struct ShaderInternal {
	unsigned int id;
};

struct OpenGLMeshInternal {
	unsigned int VAO;
	unsigned int VBO;
	unsigned int EBO;
};

void* InitGlew();
void SwapBuffers();
void MakeCtxCurrent(void* ctx);
void InitMain();

void ResizeCallback(WindowResizeEvent* e) {
	glViewport(0, 0, e->width, e->height);
	OGLReRender();
}

Shader* test_shader = nullptr;
Mesh* test_mesh = nullptr;

void OGLInit() {
	void* ctx = InitGlew();
	glm::vec2 wnd_data = Window::GetRect();
	glViewport(0, 0, wnd_data.x, wnd_data.y);
	MakeCtxCurrent(ctx);
	Event::Register<WindowResizeEvent>((EventCallback)ResizeCallback);
	glClearColor(0.5f, 0.5f, 0.5f, 1.0f);
	test_shader = CreateOGLShader({
		"assets/shaders/opengl/main.glslv",
		"assets/shaders/opengl/main.glslf"
	});
	test_mesh = CreateOGLMesh({
		-0.5f, -0.5f, 0.0f,
		0.5f, -0.5f, 0.0f,
		0.5f,  0.5f, 0.0f,
		-0.5f,  0.5f, 0.0f},{

		0, 1, 2,
		2, 3, 0
	});
}

void OGLRender() {
	glClear(GL_COLOR_BUFFER_BIT);
	glClearColor(0.5f, 0.5f, 0.5f, 1.0f);
	UseOGLShader(test_shader);
	OGLShaderSetUniformVec4(test_shader, "aColor", glm::vec4(1.0f, 1.0f, 0.0f, 1.0f));
	RenderOGLMesh(test_mesh);
	SwapBuffers();
}

void OGLReRender() {
	OGLRender();
}

void OGLShutDown() {
	DestroyOGLShader(test_shader);
	DestroyOGLMesh(test_mesh);
}

unsigned int LoadShader(std::string path, unsigned int type) {
	std::cout << "Loading shader: " << path << std::endl;
	unsigned int shader = glCreateShader(type);

	std::ifstream file(path);
	if (!file.is_open()) {
		std::cerr << "Failed to open shader file: " << path << std::endl;
		throw std::runtime_error("Failed to open shader file: " + path);
	}
	std::stringstream stream;
	stream << file.rdbuf();
	file.close();
	std::string shader_string = stream.str();
	const char* shader_src = shader_string.c_str();

	glShaderSource(shader, 1, &shader_src, nullptr);
	glCompileShader(shader);

	int success;
	glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
	if (!success) {
		char info_log[512];
		glGetShaderInfoLog(shader, 512, nullptr, info_log);
		std::cerr << "Failed to compile shader: " << info_log << std::endl;
		throw std::runtime_error("Failed to compile shader: " + std::string(info_log));
	}

	return shader;
}

Shader* CreateOGLShader(std::vector<std::string> paths) {
	std::cout << "CreateOGLShader" << std::endl;
	unsigned int program = glCreateProgram();
	std::vector<unsigned int> shaders;
	for (auto& file_path : paths) {
		unsigned int shader_type = 0;
		if (file_path.find(".glslv") != std::string::npos) {
			shader_type = GL_VERTEX_SHADER;
			std::cout << "Vertex shader: " << file_path << std::endl;
		}
		else if (file_path.find(".glslf") != std::string::npos) {
			shader_type = GL_FRAGMENT_SHADER;
			std::cout << "Fragment shader: " << file_path << std::endl;
		}
		else if (file_path.find(".glslc") != std::string::npos) {
			shader_type = GL_COMPUTE_SHADER;
			std::cout << "Compute shader: " << file_path << std::endl;
		}
		else {
			throw std::runtime_error("Shaders should only be of type glslv, glslf or glslc");
		}
		unsigned int shader = LoadShader(file_path, shader_type);
		glAttachShader(program, shader);
		shaders.push_back(shader);
	}
	glLinkProgram(program);
	int success;
	glGetProgramiv(program, GL_LINK_STATUS, &success);
	if (!success) {
		char info_log[512];
		glGetProgramInfoLog(program, 512, nullptr, info_log);
		throw std::runtime_error("Failed to link shader program: " + std::string(info_log));
	}
	for (auto& shader : shaders) {
		glDeleteShader(shader);
	}
	ShaderInternal* data = new ShaderInternal();
	data->id = program;
	Shader* ret = new Shader();
	ret->data = new ShaderInternal();
	((ShaderInternal*)ret->data)->id = program;
	return ret;
}

Mesh* CreateOGLMesh(std::vector<float> vertices, std::vector<unsigned int> indices) {
  std::cout << "CreateOGLMesh" << std::endl;
  std::cout << "vertices: " << vertices.size() << std::endl;
  std::cout << "indices: " << indices.size() << std::endl;
  Mesh* mesh = new Mesh();
  OpenGLMeshInternal* internal = new OpenGLMeshInternal();

  glGenVertexArrays(1, &internal->VAO);
  std::cout << "VAO: " << internal->VAO << std::endl;
  glBindVertexArray(internal->VAO);

  glGenBuffers(1, &internal->VBO);
  std::cout << "VBO: " << internal->VBO << std::endl;
  glBindBuffer(GL_ARRAY_BUFFER, internal->VBO);
  glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(float), vertices.data(), GL_STATIC_DRAW);

  glEnableVertexAttribArray(0);
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);

  glGenBuffers(1, &internal->EBO);
  std::cout << "EBO: " << internal->EBO << std::endl;
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, internal->EBO);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), indices.data(), GL_STATIC_DRAW);

  glBindVertexArray(0);
  mesh->indices_count = indices.size();
  mesh->indices = indices.data();
  mesh->data = internal;
  return mesh;
}

void DestroyOGLShader(Shader* shader) {
	glDeleteProgram(((ShaderInternal*)shader->data)->id);
	delete (ShaderInternal*)shader->data;
	delete shader;
}

void DestroyOGLMesh(Mesh* mesh) {
	glDeleteVertexArrays(1, &((OpenGLMeshInternal*)mesh->data)->VAO);
	glDeleteBuffers(1, &((OpenGLMeshInternal*)mesh->data)->VBO);
	glDeleteBuffers(1, &((OpenGLMeshInternal*)mesh->data)->EBO);
	delete mesh->data;
	delete mesh;
}


void UseOGLShader(Shader* shader) {
	glUseProgram(((ShaderInternal*)shader->data)->id);
}

void RenderOGLMesh(Mesh* mesh) {
	glBindVertexArray(((OpenGLMeshInternal*)mesh->data)->VAO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ((OpenGLMeshInternal*)mesh->data)->EBO);
	glDrawElements(GL_TRIANGLES, mesh->indices_count, GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);
}

void OGLShaderSetUniformBool(Shader* shader, const std::string& name, bool value) {
	glUniform1i(glGetUniformLocation(((ShaderInternal*)shader->data)->id, name.c_str()), value);
}

void OGLShaderSetUniformInt(Shader* shader, const std::string& name, int value) {
	glUniform1i(glGetUniformLocation(((ShaderInternal*)shader->data)->id, name.c_str()), value);
}

void OGLShaderSetUniformFloat(Shader* shader, const std::string& name, float value) {
	glUniform1f(glGetUniformLocation(((ShaderInternal*)shader->data)->id, name.c_str()), value);
}

void OGLShaderSetUniformVec2(Shader* shader, const std::string& name, const glm::vec2& value) {
	glUniform2f(glGetUniformLocation(((ShaderInternal*)shader->data)->id, name.c_str()), value.x, value.y);
}


void OGLShaderSetUniformVec3(Shader* shader, const std::string& name, const glm::vec3& value) {
	glUniform3f(glGetUniformLocation(((ShaderInternal*)shader->data)->id, name.c_str()), value.x, value.y, value.z);
}

void OGLShaderSetUniformVec4(Shader* shader, const std::string& name, const glm::vec4& value) {
	glUniform4f(glGetUniformLocation(((ShaderInternal*)shader->data)->id, name.c_str()), value.x, value.y, value.z, value.w);
}

void OGLShaderSetUniformMat2(Shader* shader, const std::string& name, const glm::mat2& mat) {
	glUniformMatrix2fv(glGetUniformLocation(((ShaderInternal*)shader->data)->id, name.c_str()), 1, GL_FALSE, glm::value_ptr(mat));
}

void OGLShaderSetUniformMat3(Shader* shader, const std::string& name, const glm::mat3& mat) {
	glUniformMatrix3fv(glGetUniformLocation(((ShaderInternal*)shader->data)->id, name.c_str()), 1, GL_FALSE, glm::value_ptr(mat));
}

void OGLShaderSetUniformMat4(Shader* shader, const std::string& name, const glm::mat4& mat) {
	glUniformMatrix4fv(glGetUniformLocation(((ShaderInternal*)shader->data)->id, name.c_str()), 1, GL_FALSE, glm::value_ptr(mat));
}
