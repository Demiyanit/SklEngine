#include <stdexcept>
#include <glew/glew.h>
#include <renderer/OpenGLRenderer.hpp>
#include <renderer/OpenGL/OpenGLSHader.hpp>
#include <renderer/OpenGL/OpenGLMesh.hpp>
#include <core/EngineEvents.hpp>
#include <core/Input.hpp>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

void ResizeCallback(WindowResizeEvent* e) {
	glViewport(0, 0, e->width, e->height);
	Window::renderer_instance->ReRender();
}

void CameraInputevent(InputKeyEvent* e);

class Camera {
public:

	static Camera* instance;
	static Camera*& Get() {
		return instance;
	}

	Camera() : m_projection_matrix(1.0f), m_view_matrix(1.0f) {
		instance = this;
		Event::Register<InputKeyEvent>((EventCallback)CameraInputevent);
	}

	void SetProjection(float fov, float aspect, float near, float far) {
		m_projection_matrix = glm::perspective(fov, aspect, near, far);
	}

	void SetView(glm::vec3 position, glm::vec3 target, glm::vec3 up) {
		m_position = position;
		m_target = target;
		m_up = up;
		UpdateViewMatrix();
	}

	void SetPosition(glm::vec3 position) {
		m_position = position;
		UpdateViewMatrix();
	}

	void SetRotate(glm::vec3 rotation) {
		m_rotation = rotation;
		UpdateViewMatrix();
	}

	void Move(glm::vec3 delta) {
		m_position += delta;
		UpdateViewMatrix();
	}

	void Rotate(glm::vec3 delta) {
		m_rotation += delta;
		UpdateViewMatrix();
	}

	glm::mat4 GetViewProjectionMatrix() const {
		return m_projection_matrix * m_view_matrix;
	}

private:
	void UpdateViewMatrix() {
		glm::vec3 direction = glm::normalize(m_target - m_position);
		glm::vec3 right = glm::normalize(glm::cross(direction, m_up));
		glm::vec3 up = glm::cross(right, direction);

		glm::mat4 rotation = glm::mat4(1.0f);
		rotation = glm::rotate(rotation, glm::radians(m_rotation.x), glm::vec3(1.0f, 0.0f, 0.0f));
		rotation = glm::rotate(rotation, glm::radians(m_rotation.y), glm::vec3(0.0f, 1.0f, 0.0f));
		rotation = glm::rotate(rotation, glm::radians(m_rotation.z), glm::vec3(0.0f, 0.0f, 1.0f));

		glm::vec3 translation = -glm::vec3(glm::transpose(rotation) * glm::vec4(m_position, 1.0f));

		m_view_matrix = glm::lookAt(m_position, m_target, m_up);
		m_view_matrix = rotation * glm::translate(glm::mat4(1.0f), translation) * m_view_matrix;
	}

public:
	glm::mat4 m_projection_matrix;
	glm::mat4 m_view_matrix;
	glm::vec3 m_position;
	glm::vec3 m_target;
	glm::vec3 m_up;
	glm::vec3 m_rotation;
};

Camera* Camera::instance=nullptr;

void CameraInputevent(InputKeyEvent* e) { 
	if (e->key == KEY_W) {
		Camera::Get()->Move(glm::vec3(0.0f, 0.0f, -0.05f));
	} else if (e->key == KEY_S) {
		Camera::Get()->Move(glm::vec3(0.0f, 0.0f, 0.05f));
	} else if (e->key == KEY_A) {
		Camera::Get()->Move(glm::vec3(-0.05f, 0.0f, 0.0f));
	} else if (e->key == KEY_D) {
		Camera::Get()->Move(glm::vec3(0.05f, 0.0f, 0.0f));
	} else if (e->key == KEY_LEFT) {
		Camera::Get()->Rotate(glm::vec3(0.0f, -0.05f, 0.0f));
	} else if (e->key == KEY_RIGHT) {
		Camera::Get()->Rotate(glm::vec3(0.0f, 0.05f, 0.0f));
	} else if (e->key == KEY_UP) {
		Camera::Get()->Rotate(glm::vec3(-0.05f, 0.0f, 0.0f));
	} else if (e->key == KEY_DOWN) {
		Camera::Get()->Rotate(glm::vec3(0.05f, 0.0f, 0.0f));
	} else if (e->key == KEY_Q) {
		Camera::Get()->Rotate(glm::vec3(0.0f, 0.0f, -0.05f));
	} else if (e->key == KEY_E) {
		Camera::Get()->Rotate(glm::vec3(0.0f, 0.0f, 0.05f));
	}
}



OpenGLRenderer::OpenGLRenderer() {
try {
	this->InitGlew();
	glm::vec2 wnd_data = Window::GetRect();
	glViewport(0, 0, wnd_data.x, wnd_data.y);
	this->MakeCtxCurrent();
	Event::Register<WindowResizeEvent>((EventCallback)ResizeCallback);
	glClearColor(0.5f, 0.5f, 0.5f, 1.0f);
	this->shader = new OpenGLShader();
	this->shader->Construct({
		"assets/shaders/opengl/main.glslv",
		"assets/shaders/opengl/main.glslf" 
	});
	this->mesh = new OpenGLMesh();
	this->mesh->Create({
		-0.5f, -0.5f, 0.0f,
		0.5f, -0.5f, 0.0f,
		0.5f,  0.5f, 0.0f,
		-0.5f,  0.5f, 0.0f
	}, {
		0, 1, 2,
		2, 3, 0
	});
	
	this->camera = new Camera();
}
catch (std::runtime_error e) {
	throw e;
}}

void OpenGLRenderer::Render() {
	glClear(GL_COLOR_BUFFER_BIT);
	this->shader->Use();
	this->shader->SetUniformMat4("view_matrix", camera->m_view_matrix);
	this->shader->SetUniformVec4("aColor", glm::vec4(1.0f, 1.0f, 0.0f, 1.0f));
	this->mesh->Render();
	this->shader->UnUse();
	this->SwapBuffers();
}


void OpenGLRenderer::ReRender() {
	Render();
}

OpenGLRenderer::~OpenGLRenderer() {
	delete this->mesh;
	delete this->shader;
}

