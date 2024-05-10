#include <platform/Window.hpp>
#include <renderer/IRenderer.hpp>
#include <string>

class OpenGLShader;
class OpenGLMesh;
class OpenGLRenderer : public IRenderer {
public:

	OpenGLRenderer();
	void Render() override;
	void ReRender() override;
	~OpenGLRenderer() override;
private:
	void InitGlew();
	void SwapBuffers();
	void MakeCtxCurrent();
	void InitMain();
	void* ctx = nullptr;
	OpenGLShader* shader = nullptr;
	OpenGLMesh*   mesh   = nullptr;
	class Camera* camera = nullptr;
};