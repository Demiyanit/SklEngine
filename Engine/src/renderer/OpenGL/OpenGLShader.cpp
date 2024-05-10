#include <fstream>
#include <sstream>
#include <stdexcept>

#include <renderer/OpenGL/OpenGLSHader.hpp>
#include <glew/glew.h>
#include <unordered_map>
#include <glm/gtc/type_ptr.hpp>

void OpenGLShader::Construct(std::vector<std::string> shader_file_paths) {
	unsigned int program = glCreateProgram();
	std::vector<unsigned int> shaders;
	for (auto& file_path : shader_file_paths) {
		unsigned int shader_type = 0;
		if (file_path.find(".glslv") != std::string::npos) {
			shader_type = GL_VERTEX_SHADER;
		}
		else if (file_path.find(".glslf") != std::string::npos) {
			shader_type = GL_FRAGMENT_SHADER;
		}
		else if (file_path.find(".glslc") != std::string::npos) {
			shader_type = GL_COMPUTE_SHADER;
		}
		else {
			throw new std::runtime_error("Shaders should only be of type glslv, glslf or glslc");
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
	this->program_id = program;
}

void OpenGLShader::Use() {
	glUseProgram(this->program_id);
}

void OpenGLShader::UnUse() {
	glUseProgram(GL_NONE);
}

unsigned int OpenGLShader::LoadShader(std::string path, unsigned int type) {
	unsigned int shader = glCreateShader(type);

	std::ifstream file(path);
	if (!file.is_open()) {
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
		throw std::runtime_error("Failed to compile shader: " + std::string(info_log));
	}

	return shader;		
}


std::unordered_map<std::string, int> uniform_location_cache;

void OpenGLShader::SetUniformBool(const std::string& name, bool value) {
	glUniform1i(GetUniformLocation(name), (int)value);
}

void OpenGLShader::SetUniformInt(const std::string& name, int value) {
	glUniform1i(GetUniformLocation(name), value);
}

void OpenGLShader::SetUniformFloat(const std::string& name, float value) {
	glUniform1f(GetUniformLocation(name), value);
}

void OpenGLShader::SetUniformVec2(const std::string& name, const glm::vec2& value) {
	glUniform2f(GetUniformLocation(name), value.x, value.y);
}

void OpenGLShader::SetUniformVec2(const std::string& name, float x, float y) {
	glUniform2f(GetUniformLocation(name), x, y);
}

void OpenGLShader::SetUniformVec3(const std::string& name, const glm::vec3& value) {
	glUniform3f(GetUniformLocation(name), value.x, value.y, value.z);
}

void OpenGLShader::SetUniformVec3(const std::string& name, float x, float y, float z) {
	glUniform3f(GetUniformLocation(name), x, y, z);
}

void OpenGLShader::SetUniformVec4(const std::string& name, const glm::vec4& value) {
	glUniform4f(GetUniformLocation(name), value.x, value.y, value.z, value.w);
}

void OpenGLShader::SetUniformVec4(const std::string& name, float x, float y, float z, float w) {
	glUniform4f(GetUniformLocation(name), x, y, z, w);
}

void OpenGLShader::SetUniformMat2(const std::string& name, const glm::mat2& mat) {
	glUniformMatrix2fv(GetUniformLocation(name), 1, GL_FALSE, glm::value_ptr(mat));
}

void OpenGLShader::SetUniformMat3(const std::string& name, const glm::mat3& mat) {
	glUniformMatrix3fv(GetUniformLocation(name), 1, GL_FALSE, glm::value_ptr(mat));
}

void OpenGLShader::SetUniformMat4(const std::string& name, const glm::mat4& mat) {
	glUniformMatrix4fv(GetUniformLocation(name), 1, GL_FALSE, glm::value_ptr(mat));
}

int OpenGLShader::GetUniformLocation(const std::string& name) {
	if (uniform_location_cache.find(name) == uniform_location_cache.end()) {
		int location = glGetUniformLocation(this->program_id, name.c_str());
		if (location == -1) {
			throw std::runtime_error("Invalid uniform location");
		}
		uniform_location_cache[name] = location;
	}
	return uniform_location_cache[name];
}

