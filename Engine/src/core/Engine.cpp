#include <iostream>
#include <stdexcept>
#include <core/Application.hpp>
#include <core/EngineEvents.hpp>
#include <core/Engine.hpp>
#include <platform/Window.hpp>
#include <core/Input.hpp>
#include <renderer/Renderer.hpp>
#include <util/SceneUtils.hpp>
IApplication* Engine::application_instance = nullptr;

void InitShutdownSequence(EngineCloseEvent* e) {
	Window::SetShouldClose(true);
	if (e->error_code < 0) {
		std::cerr << "Error: " << e->message << std::endl;
		return;
	}
	std::cout << e->message << std::endl;
}

void Engine::Initialize(IApplication* inst) {
	Engine::application_instance = inst;
	
	try {
		Window::Init(inst->ApplicationName.c_str(), inst->width, inst->height);
		Input::Init();
		Renderer::Get(OpenGL);
		Renderer::Initialize();
		Event::Register<EngineCloseEvent>((EventCallback)InitShutdownSequence);
	}
	catch (std::exception e) {
		std::cerr << "[ERROR]: Engine failed to initialize... \n" << e.what() << "\nExiting..." << std::endl;
		exit(-1);
	}
}
Scene test = Scene();
int Engine::Run() {
try {
	GameObject test_obj;
	test_obj.render_data;
	test_obj.render_data.main_shader = Renderer::CreateShader({ "assets/shaders/opengl/main.glslv","assets/shaders/opengl/main.glslf" });
	std::vector<float> vertices = {
    // Front face
    -0.5f, -0.5f,  0.5f,   // 0
     0.5f, -0.5f,  0.5f,   // 1
     0.5f,  0.5f,  0.5f,   // 2
    -0.5f,  0.5f,  0.5f,   // 3

    // Back face
    -0.5f, -0.5f, -0.5f,  // 4
     0.5f, -0.5f, -0.5f,  // 5
     0.5f,  0.5f, -0.5f,  // 6
    -0.5f,  0.5f, -0.5f,  // 7

    // Right face
     0.5f, -0.5f,  0.5f,   // 1
     0.5f, -0.5f, -0.5f,   // 5
     0.5f,  0.5f, -0.5f,   // 6
     0.5f,  0.5f,  0.5f,   // 2

    // Left face
    -0.5f, -0.5f,  0.5f,   // 0
    -0.5f, -0.5f, -0.5f,   // 4
    -0.5f,  0.5f, -0.5f,   // 7
    -0.5f,  0.5f,  0.5f,   // 3

    // Top face
    -0.5f,  0.5f,  0.5f,   // 3
     0.5f,  0.5f,  0.5f,   // 2
     0.5f,  0.5f, -0.5f,   // 6
    -0.5f,  0.5f, -0.5f,   // 7

    // Bottom face
    -0.5f, -0.5f,  0.5f,   // 0
     0.5f, -0.5f,  0.5f,   // 1
     0.5f, -0.5f, -0.5f,   // 5
    -0.5f, -0.5f, -0.5f,   // 4
};

std::vector<unsigned int> indices = {
    0, 1, 2, 2, 3, 0,  // Front face
    4, 5, 6, 6, 7, 4,  // Back face
    8, 9, 10, 10, 11, 8,  // Right face
    12, 13, 14, 14, 15, 12,  // Left face
    16, 17, 18, 18, 19, 16,  // Top face
    20, 21, 22, 22, 23, 20   // Bottom face
};

test_obj.render_data.object_mesh = Renderer::CreateMesh(vertices, indices);

	test_obj.render_data.object_matrix = glm::mat4(1.0f);
	test_obj.transform.position = glm::vec3(0.0f);
	test_obj.transform.rotation = glm::vec3(0.0f, 50.0f, 45.0f);
	test_obj.transform.scale = glm::vec3(1.0f);
	test.main.push_back(test_obj);
	Engine::application_instance->OnInit();
	while(!Window::ShouldClose()) {
		Window::Update();
		Input::Update();
		application_instance->OnRender();
		// Rotate the game object
		test.main[0].transform.rotation.x += 0.001f; // Rotate by 0.01 radians (approximately 0.6 degrees)
		test.main[0].transform.rotation.y += 0.001f; // Rotate by 0.01 radians (approximately 0.6 degrees)
		test.main[0].transform.rotation.z += 0.001f; // Rotate by 0.01 radians (approximately 0.6 degrees)

		// Floor the rotation values to 360 degrees
		test.main[0].transform.rotation.x = test.main[0].transform.rotation.x > 360.0f ? 0.0f : test.main[0].transform.rotation.x;
		test.main[0].transform.rotation.y = test.main[0].transform.rotation.y > 360.0f ? 0.0f : test.main[0].transform.rotation.y;
		test.main[0].transform.rotation.z = test.main[0].transform.rotation.z > 360.0f ? 0.0f : test.main[0].transform.rotation.z;

		test.Render();
	}
	Engine::application_instance->OnShutdown();
	Renderer::DestroyShader(&test_obj.render_data.main_shader);
	Renderer::DestroyMesh(&test_obj.render_data.object_mesh);
}
catch (std::exception e) {
	std::cerr << "[ERROR]: Engine failed to update...\n" << e.what() << "\n Shutting down..." << std::endl;
	Engine::application_instance->OnShutdown();
	return -1;
}return 0;
}

void Engine::Shutdown() {
	Window::Shutdown();
}
