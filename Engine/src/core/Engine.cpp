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

#include <vector>
#include <fstream>
#include <sstream>
#include <string>

class Obj {
public:
    std::vector<float> vertices;
    std::vector<unsigned int> indices;

    bool LoadFromObjFile(const std::string& filename) {
        std::ifstream file(filename);
        if (!file.is_open()) {
            return false;
        }

        std::string line;
        while (std::getline(file, line)) {
            std::istringstream iss(line);
            std::string prefix;
            iss >> prefix;

            if (prefix == "v") {
                float x, y, z;
                iss >> x >> y >> z;
                vertices.push_back(x);
                vertices.push_back(y);
                vertices.push_back(z);
            }
            else if (prefix == "f") {
                std::string vertexIndices[3];
                iss >> vertexIndices[0] >> vertexIndices[1] >> vertexIndices[2];

                for (int i = 0; i < 3; ++i) {
                    std::size_t pos = vertexIndices[i].find("/");
                    vertexIndices[i] = vertexIndices[i].substr(0, pos);
                    unsigned int index = std::stoi(vertexIndices[i]) - 1;
                    indices.push_back(index);
                }
            }
        }

        return true;
    }
};

Scene test = Scene();
int Engine::Run() {
try {
    Obj obj;
    obj.LoadFromObjFile("../../assets/test.obj");
	GameObject test_obj;
	Shader* sh = new Shader();
	*sh = Renderer::CreateShader
	({ "../../assets/shaders/opengl/main.glslv",
		"../../assets/shaders/opengl/main.glslf" });
	test_obj.render_data.main_shader = sh;
	
	Mesh* mh = new Mesh();
	*mh = Renderer::CreateMesh(obj.vertices, obj.indices);

	test_obj.render_data.object_mesh = mh;
		
	test_obj.render_data.object_matrix = glm::mat4(1.0f);
	test_obj.render_data.color = glm::vec4(1.0f);
	test_obj.transform.position = glm::vec3(0.0f);
	test_obj.transform.rotation = glm::vec3(0.0f, 0.0f, 0.0f);
	test_obj.transform.scale = glm::vec3(1.0f);
	test.main.push_back(test_obj);

	Camera test_cam = Camera();
	test_cam.clear_color = glm::vec4(0.2f, 0.2f, 0.3f, 1.0f);
	test_cam.viewport_pos = glm::vec4(80.0f, 60.0f, Window::GetRect().x, Window::GetRect().y);
	test_cam.old_viewport_pos = glm::vec4(0);
	test_cam.transform.position = glm::vec3(0.0f, 0.0f, 10.0f);
	test_cam.transform.rotation = glm::vec3(0.0f, 0.0f, 0.0f);
	Engine::application_instance->OnInit();
	while(!Window::ShouldClose()) {
		Window::Update();
		Input::Update();
		if (Input::KeyPressed(KEY_ESCAPE))
			Window::SetShouldClose(true);
		// Move the camera with WASD
		if (Input::KeyPressed(KEY_W)) {
			glm::mat4 iv = glm::inverse(test_cam.view);
			glm::vec3 forward = normalize(glm::vec3(iv[2]));

			test_cam.transform.position -= forward * 1.0f;
		}
		if(Input::KeyPressed(KEY_S)){
			glm::mat4 iv = glm::inverse(test_cam.view);
			glm::vec3 forward = normalize(glm::vec3(iv[2]));

			test_cam.transform.position += forward * 1.0f;
		}
		if(Input::KeyPressed(KEY_A)) {
			glm::mat4 iv = glm::inverse(test_cam.view);
			glm::vec3 right = normalize(glm::vec3(iv[0]));
			test_cam.transform.position -= right * 1.0f;
		}
		if(Input::KeyPressed(KEY_D)) {
			glm::mat4 iv = glm::inverse(test_cam.view);
			glm::vec3 right = normalize(glm::vec3(iv[0]));
			test_cam.transform.position += right * 1.0f;
		}
		
		static bool locked = true;
		
		// Track mouse movement
		double xoffset = Input::mouse_delta.x;
		double yoffset = Input::mouse_delta.y;
		if (locked) {
			test_cam.transform.rotation.x -= (float)yoffset * 0.1f;
			test_cam.transform.rotation.y -= (float)xoffset * 0.1f;
		}
		
		application_instance->OnRender();
		
		if (Input::KeyPressed(KEY_J))
			test.main[0].transform.rotation.x += glm::radians(10.0f); 
		if (Input::KeyPressed(KEY_L))
			test.main[0].transform.rotation.x -= glm::radians(10.0f); 
		if (Input::KeyPressed(KEY_I))
			test.main[0].transform.rotation.y += glm::radians(10.0f); 
		if (Input::KeyPressed(KEY_K))
			test.main[0].transform.rotation.y -= glm::radians(10.0f); 
		if (Input::KeyPressed(KEY_B))
			test.main[0].transform.rotation.z += glm::radians(10.0f);
		if (Input::KeyPressed(KEY_H))
			test.main[0].transform.rotation.z -= glm::radians(10.0f);
		// Floor the rotation values to 360 degrees
		test.main[0].transform.rotation.x = test.main[0].transform.rotation.x > 360.0f ? 0.0f : test.main[0].transform.rotation.x;
		test.main[0].transform.rotation.y = test.main[0].transform.rotation.y > 360.0f ? 0.0f : test.main[0].transform.rotation.y;
		test.main[0].transform.rotation.z = test.main[0].transform.rotation.z > 360.0f ? 0.0f : test.main[0].transform.rotation.z;

		std::vector<RenderData> renderData = test.ConstructRenderData();
		Renderer::StartRender();
		test_cam.Render(renderData);
		Renderer::FinishRender();
	}
	Engine::application_instance->OnShutdown();
	Renderer::DestroyShader(test_obj.render_data.main_shader);
	Renderer::DestroyMesh(test_obj.render_data.object_mesh);
	delete mh;
	delete sh;
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
