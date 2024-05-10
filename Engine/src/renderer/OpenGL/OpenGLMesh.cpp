#include <renderer/OpenGL/OpenGLMesh.hpp>
#include <glew/glew.h>
#include <stdexcept>



void OpenGLMesh::Create(std::vector<float> verticies, std::vector<unsigned int> indicies) {
	this->verticies = verticies;
	this->indicies = indicies;
	glGenVertexArrays(1, &VAO);
	if (VAO == 0) {
		throw std::runtime_error("Failed to create vertex array");
	}
	glBindVertexArray(VAO);

	glGenBuffers(1, &VBO);
	if (VBO == 0) {
		throw std::runtime_error("Failed to create vertex buffer");
	}
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, verticies.size() * sizeof(float), verticies.data(), GL_STATIC_DRAW);

	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);

	glGenBuffers(1, &EBO);
	if (EBO == 0) {
		throw std::runtime_error("Failed to create index buffer");
	}
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, indicies.size() * sizeof(unsigned int), indicies.data(), GL_STATIC_DRAW);

	glBindVertexArray(0);
}

void OpenGLMesh::Render() {
	glBindVertexArray(VAO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glDrawElements(GL_TRIANGLES, indicies.size(), GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);
}
