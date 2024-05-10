#pragma once
#include <renderer/IShader.hpp>

class OpenGLShader : public IShader {
public:
	void Construct(std::vector<std::string> shader_file_paths)override;
	void Use()override;
	void UnUse()override;
	void SetUniformBool(const std::string& name, bool value) override;
	void SetUniformInt(const std::string& name, int value) override;
	void SetUniformFloat(const std::string& name, float value) override;
	void SetUniformVec2(const std::string& name, const glm::vec2& value) override;
	void SetUniformVec2(const std::string& name, float x, float y) override;
	void SetUniformVec3(const std::string& name, const glm::vec3& value) override;
	void SetUniformVec3(const std::string& name, float x, float y, float z) override;
	void SetUniformVec4(const std::string& name, const glm::vec4& value) override;
	void SetUniformVec4(const std::string& name, float x, float y, float z, float w) override;
	void SetUniformMat2(const std::string& name, const glm::mat2& mat) override;
	void SetUniformMat3(const std::string& name, const glm::mat3& mat) override;
	void SetUniformMat4(const std::string& name, const glm::mat4& mat) override;

private:
	int GetUniformLocation(const std::string& name);
	unsigned int LoadShader(std::string path, unsigned int type);
	unsigned int program_id;
};
