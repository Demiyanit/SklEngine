#include <platform/Window.hpp>
#include <renderer/IRenderer.hpp>
#include <string>

class OpenGLRenderer : public IRenderer {
public:
	OpenGLRenderer();
	void Render() override;
	~OpenGLRenderer() override;
private:
	void InitGlew();
	void SwapBuffers();
	void MakeCtxCurrent();
	void InitMain();
	void* ctx = nullptr;
};