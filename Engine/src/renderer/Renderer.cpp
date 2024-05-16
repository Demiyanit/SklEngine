#include <renderer/Renderer.hpp>
#include <renderer/OpenGL/OpenGL.hpp>
void (*Renderer::Initialize)()  =0;
void (*Renderer::StartRender)() =0;
void (*Renderer::FinishRender)() =0;
void (*Renderer::Render)(glm::mat4 projection, glm::mat4 view, RenderData data)  =0;
void (*Renderer::SetViewPort)  (int x, int y, int width, int height)=0;
void (*Renderer::ClearColor)(glm::vec4 color);
Mesh (*Renderer::CreateMesh)(std::string path, std::vector<float>* vertices, std::vector<unsigned int>* indices);
void (*Renderer::DestroyMesh)(Mesh* mesh);
Shader (*Renderer::CreateShader)(std::vector<std::string> paths);
void (*Renderer::DestroyShader)(Shader* shader);
Texture (*Renderer::CreateTexture)(std::string path);
void (*Renderer::DestroyTexture)(Texture* texture);
glm::mat4 Renderer::proj = glm::mat4(1.0f);
void Renderer::Get(RendererType renderer) {
	switch (renderer) {
	case OpenGL:
		Renderer::Initialize = OGLInit;
		Renderer::StartRender = OGLStartRender;
		Renderer::Render = OGLRender;
		Renderer::FinishRender = OGLFinishRender;
		Renderer::SetViewPort = OGLSetViewport;
		Renderer::ClearColor = OGLClearColor;
		Renderer::CreateMesh = CreateOGLMesh;
		Renderer::DestroyMesh = DestroyOGLMesh;
		Renderer::CreateShader = CreateOGLShader;
		Renderer::DestroyShader = DestroyOGLShader;
		Renderer::CreateTexture = CreateOGLTexture;
		Renderer::DestroyTexture = DestroyOGLTexture;
		break;
	case Dx:
		break;
	case Vulkan:
		break;
	default:
		break;
	}
}
