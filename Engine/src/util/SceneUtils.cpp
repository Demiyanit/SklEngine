#include <fstream>

#include <pugixml/pugixml.hpp>

#include <util/SceneUtils.hpp>

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/euler_angles.hpp>

std::vector<RenderData> Scene::ConstructRenderData() {
	std::vector<RenderData> renderData;
	for (GameObject gameObject : this->main) {
		RenderGameObject(&gameObject, glm::mat4(1.0f), renderData);
	}
	return renderData;
}

void Scene::RenderGameObject(GameObject* gameObject, glm::mat4 parentMatrix, std::vector<RenderData>& renderData) {
	// Calculate the model matrix for the current game object
	gameObject->render_data.object_matrix = parentMatrix * CalculateTransformMatrix(gameObject);

	// Add the render data to the render data vector
	renderData.push_back(gameObject->render_data);

	// Recursively render children of the current game object
	for (GameObject child : gameObject->children) {
		RenderGameObject(&child, gameObject->render_data.object_matrix, renderData);
	}
}

glm::mat4 Scene::CalculateTransformMatrix(GameObject* gameObject) {
  glm::mat4 rotationMatrix = glm::eulerAngleYXZ(
    glm::radians(gameObject->transform.rotation.y),
    glm::radians(gameObject->transform.rotation.x),
    glm::radians(gameObject->transform.rotation.z)
  );

  glm::mat4 objectMatrix = glm::mat4(
    gameObject->transform.scale.x, 0.0f, 0.0f, 0.0f,
    0.0f, gameObject->transform.scale.y, 0.0f, 0.0f,
    0.0f, 0.0f, gameObject->transform.scale.z, 0.0f,
    gameObject->transform.position.x, gameObject->transform.position.y, gameObject->transform.position.z, 1.0f
  );

  return glm::transpose(rotationMatrix * objectMatrix);
}

void Scene::Save(std::string path) {
	pugi::xml_document doc;
	auto decl = doc.append_child(pugi::node_declaration);
	decl.append_attribute("version") = "1.0";
	decl.append_attribute("encoding") = "UTF-8";
	{
		auto Engine = doc.append_child("Engine");
		auto Shaders = Engine.append_child("Shaders");
		auto Textures = Engine.append_child("Textures");
		auto Meshes = Engine.append_child("Meshes");
	}
	auto sceneNode = doc.append_child("Scene");
	for (const GameObject object : this->main) {
		pugi::xml_node objectNode = sceneNode.append_child("GameObject");
		object.Save(&objectNode, &sceneNode);
	}

	doc.save_file(path.c_str());
}

void GameObject::Save(pugi::xml_node* node, pugi::xml_node* scene_node) const {
	node->append_attribute("name") = name.c_str();
	node->append_attribute("tag") = tag.c_str();
	node->append_attribute("active") = isActive;
	
	{	
	
		auto Shaders = scene_node->find_child([](pugi::xml_node node) { return std::string(node.name()) == "Shaders"; });
		auto Textures = scene_node->find_child([](pugi::xml_node node) { return std::string(node.name()) == "Textures"; });
		auto Meshes = scene_node->find_child([](pugi::xml_node node) { return std::string(node.name()) == "Meshes"; });
		auto Shader = scene_node->find_child_by_attribute("Shader", "uid", std::to_string(this->render_data.main_shader->uid).c_str());
		if (Shader.empty()) {
			Shader = Shaders.append_child("Shader");
			Shader.append_attribute("uid") = this->render_data.main_shader->uid;
			for(auto ShaderPath : this->render_data.main_shader->paths) {
				Shader.append_child("Path").append_child(pugi::node_pcdata).set_value(ShaderPath.c_str());
			}
		}

		auto Texture = scene_node->find_child_by_attribute("Texture", "uid", std::to_string(this->render_data.tx->uid).c_str());
		if (Textures.empty()) {
			Texture = scene_node->append_child("Texture");
			Texture.append_attribute("uid") = this->render_data.tx->uid;
			Texture.append_child("Path").append_child(pugi::node_pcdata).set_value(this->render_data.tx->path.c_str());
		}

		auto Mesh = scene_node->find_child_by_attribute("Mesh", "uid", std::to_string(this->render_data.object_mesh->uid).c_str());
		if (Mesh.empty()) {
			Mesh = scene_node->append_child("Mesh");
			Mesh.append_attribute("uid") = this->render_data.object_mesh->uid;
			Mesh.append_child("Path").append_child(pugi::node_pcdata).set_value(this->render_data.object_mesh->path.c_str());
		}
		auto renderNode = node->append_child("RenderData");
		auto renderNodeShader = renderNode.append_child("Shader");
		renderNodeShader.append_attribute("uid") = this->render_data.main_shader->uid;
		auto renderNodeTexture = renderNode.append_child("Texture");
		renderNodeTexture.append_attribute("uid") = this->render_data.tx->uid;
		auto renderNodeMesh = renderNode.append_child("Mesh");
		renderNodeMesh.append_attribute("uid") = this->render_data.object_mesh->uid;
		
	}

	{
		auto transformNode = node->append_child("Transform");
		auto pos = transformNode.append_child("position");
		pos.append_attribute("x") = transform.position.x;
		pos.append_attribute("y") = transform.position.y;
		pos.append_attribute("z") = transform.position.z;
		auto rot = transformNode.append_child("rotation");
		rot.append_attribute("x") = transform.rotation.x;
		rot.append_attribute("y") = transform.rotation.y;
		rot.append_attribute("z") = transform.rotation.z;
		auto scale = transformNode.append_child("scale");
		scale.append_attribute("x") = transform.scale.x;
		scale.append_attribute("y") = transform.scale.y;
		scale.append_attribute("z") = transform.scale.z;
	}
	
	for(const auto& child : children) {
		auto childNode = node->append_child("GameObject");
		child.Save(&childNode, scene_node);
	}
	
	auto componentsNode = node->append_child("Components");
	for (const auto& component : components) {
  		auto componentNode = componentsNode.append_child("Component");
		componentNode.append_attribute("name") = component.name.c_str();
		componentNode.append_attribute("tag") = component.tag.c_str();
		componentNode.append_attribute("active") = component.isActive;
	}
}

Scene  Scene::Load(std::string path) {
	return Scene();
}

Scene* Scene::LoadPTR(std::string path) {
	Scene sc = Load(path);
	Scene* s = new Scene();
	*s = sc;
	return s;
}

glm::mat4 Camera::CalculateViewMatrix() {
    glm::mat4 rotationMatrix = glm::eulerAngleXYZ(
      glm::radians(-this->transform.rotation.x),
      glm::radians(-this->transform.rotation.y),
      glm::radians(-this->transform.rotation.z)
    );

    glm::mat4 translationMatrix = glm::translate(glm::mat4(1.0f), -this->transform.position);
    glm::mat4 viewMatrix = rotationMatrix * translationMatrix;

    return viewMatrix;
}



void Camera::Render(std::vector<RenderData> data) {
	if(old_viewport_pos != viewport_pos) {
		old_viewport_pos = viewport_pos;
		float aspect_ratio = static_cast<float>(viewport_pos.z) / static_cast<float>(viewport_pos.w);
		if (aspect_ratio >= 1.7) { // 16/9
			aspect_ratio = 16.0f / 9.0f;
		}
		else if (aspect_ratio >= 1.6) { // 16/10
			aspect_ratio = 16.0f / 10.0f;
		}
		else { // 4/3
			aspect_ratio = 4.0f / 3.0f;
		}

		projection = glm::perspective(
		glm::radians(44.0f), // Field of view in radians
		aspect_ratio,
		0.01f, 10000.0f
		) ;
	}
	view = CalculateViewMatrix();
	Renderer::SetViewPort(
		viewport_pos.x, 
		viewport_pos.y, 
		viewport_pos.z, 
		viewport_pos.w
	);
	Renderer::ClearColor(this->clear_color);
	
	for (RenderData& d : data) {
		Renderer::Render(projection, view, d);
	}
	
}
