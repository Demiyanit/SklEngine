#pragma once
#include <string>
#include <vector>
#include <glm/glm.hpp>
class IShader {
public:
	virtual void Construct(std::vector<std::string> shader_file_paths) = 0;
	virtual void Use() = 0;
	virtual void UnUse() = 0;
	virtual void SetUniformBool(const std::string& name, bool value) = 0;
	virtual void SetUniformInt(const std::string& name, int value) = 0;
	virtual void SetUniformFloat(const std::string& name, float value) = 0;
	virtual void SetUniformVec2(const std::string& name, const glm::vec2& value) = 0;
	virtual void SetUniformVec2(const std::string& name, float x, float y) = 0;
	virtual void SetUniformVec3(const std::string& name, const glm::vec3& value) = 0;
	virtual void SetUniformVec3(const std::string& name, float x, float y, float z) = 0;
	virtual void SetUniformVec4(const std::string& name, const glm::vec4& value) = 0;
	virtual void SetUniformVec4(const std::string& name, float x, float y, float z, float w) = 0;
	virtual void SetUniformMat2(const std::string& name, const glm::mat2& mat) = 0;
	virtual void SetUniformMat3(const std::string& name, const glm::mat3& mat) = 0;
	virtual void SetUniformMat4(const std::string& name, const glm::mat4& mat) = 0;
};
