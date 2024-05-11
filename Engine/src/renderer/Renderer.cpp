#include <renderer/Renderer.hpp>
#include <renderer/OpenGL/OpenGL.hpp>
void (*Renderer::Initialize)()  =0;
void (*Renderer::Render)(glm::mat4 matrix, Shader* shader, Mesh* mesh)  =0;
void (*Renderer::ReRender)()=0;
void (*Renderer::Shutdown)()=0;
Shader (*Renderer::CreateShader) (std::vector<std::string> paths)=0;
void   (*Renderer::DestroyShader)(Shader* shader)=0;
Mesh   (*Renderer::CreateMesh)   (std::vector<float> vertices, std::vector<unsigned int> indices)=0;
void   (*Renderer::DestroyMesh)  (Mesh* mesh)=0;
void   (*Renderer::UseShader)  (Shader* shader)=0;
void   (*Renderer::RenderMesh) (Mesh* mesh)=0;
void (*Renderer::ShaderSetUniformBool)(Shader* shader, const std::string& name, bool value)=0;
void (*Renderer::ShaderSetUniformInt)(Shader* shader, const std::string& name, int value)=0;
void (*Renderer::ShaderSetUniformFloat)(Shader* shader, const std::string& name, float value)=0;
void (*Renderer::ShaderSetUniformVec2)(Shader* shader, const std::string& name, const glm::vec2& value)=0;
void (*Renderer::ShaderSetUniformVec3)(Shader* shader, const std::string& name, const glm::vec3& value)=0;
void (*Renderer::ShaderSetUniformVec4)(Shader* shader, const std::string& name, const glm::vec4& value)=0;
void (*Renderer::ShaderSetUniformMat2)(Shader* shader, const std::string& name, const glm::mat2& mat)=0;
void (*Renderer::ShaderSetUniformMat3)(Shader* shader, const std::string& name, const glm::mat3& mat)=0;
void (*Renderer::ShaderSetUniformMat4)(Shader* shader, const std::string& name, const glm::mat4& mat)=0;

void Renderer::Get(RendererType renderer) {
	switch (renderer) {
	case OpenGL:
		Renderer::Initialize = OGLInit;
		Renderer::Render = OGLRender;
		Renderer::ReRender = OGLReRender;
		Renderer::Shutdown = OGLShutDown;
		Renderer::CreateShader = CreateOGLShader;
		Renderer::DestroyShader = DestroyOGLShader;
		Renderer::CreateMesh = CreateOGLMesh;
		Renderer::DestroyMesh = DestroyOGLMesh;
		Renderer::UseShader = UseOGLShader;
		Renderer::RenderMesh = RenderOGLMesh;
		Renderer::ShaderSetUniformBool = OGLShaderSetUniformBool;
		Renderer::ShaderSetUniformInt = OGLShaderSetUniformInt;
		Renderer::ShaderSetUniformFloat = OGLShaderSetUniformFloat;
		Renderer::ShaderSetUniformVec2 = OGLShaderSetUniformVec2;
		Renderer::ShaderSetUniformVec3 = OGLShaderSetUniformVec3;
		Renderer::ShaderSetUniformVec4 = OGLShaderSetUniformVec4;
		Renderer::ShaderSetUniformMat2 = OGLShaderSetUniformMat2;
		Renderer::ShaderSetUniformMat3 = OGLShaderSetUniformMat3;
		Renderer::ShaderSetUniformMat4 = OGLShaderSetUniformMat4;
		break;
	case Dx:
		break;
	case Vulkan:
		break;
	default:
		break;
	}
}
