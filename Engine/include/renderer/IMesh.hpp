#pragma once

#include <vector>

class IMesh {
public:
	virtual void Create(std::vector<float> verticies, std::vector<unsigned int> indicies) = 0;
	virtual void Render()=0;
protected:
	std::vector<float> verticies;
	std::vector<unsigned int> indicies;
};