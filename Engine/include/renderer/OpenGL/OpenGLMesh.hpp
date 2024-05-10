#pragma once
#include <renderer/IMesh.hpp>

class OpenGLMesh : public IMesh {
public:
	void Create(std::vector<float> verticies, std::vector<unsigned int> indicies) override;
	void Render() override;
private:
	unsigned int VAO = 0;
	unsigned int VBO = 0;
	unsigned int EBO = 0;
};