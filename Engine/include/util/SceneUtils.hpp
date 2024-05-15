#pragma once

#include <vector>
#include <string>
#include <typeindex>
#include <renderer/Renderer.hpp>

class GameObject;

class Transform {
public:
	GameObject* obj;
	glm::vec3 position;
	glm::vec3 rotation;
	glm::vec3 scale;
};

namespace pugi {
	class xml_node;
};

class Component {
public:
	virtual std::type_index Type() { return typeid(*this); }
	virtual void Save(pugi::xml_node* node, pugi::xml_node* scene_node);
	GameObject* parent;
	Transform transform;
	bool isActive = true;
	std::string name;
	std::string tag;
};

class GameObject {
public:
	GameObject* parent;
	RenderData render_data;
	Transform transform;
	bool isActive = true;
	std::string tag;
	std::string name;
	//Both of them should store allocated instances for virtual shit
	std::vector<std::shared_ptr<GameObject>> children;
	std::vector<std::shared_ptr<Component>> components;

	void Save(pugi::xml_node* node, pugi::xml_node* scene_node) const;
private:
	friend class Scene;
};

class Camera {
public:
	Transform transform;
	glm::vec4 clear_color;
	glm::vec4 viewport_pos;
	glm::vec4 old_viewport_pos;
	glm::mat4 projection;
	glm::mat4 view;
	glm::mat4 CalculateViewMatrix();
	void Render(std::vector<RenderData> data);
};

class Scene {
public:
	std::string name;
	std::vector<RenderData> ConstructRenderData();

	void Save(std::string path);
	static Scene  Load   (std::string path);
	static Scene* LoadPTR(std::string path);
	std::vector<GameObject> main;
	std::vector<std::shared_ptr<Component>> scene_components;
private:
	void RenderGameObject(GameObject* gameObject, glm::mat4 parentMatrix, std::vector<RenderData>& renderData);
	glm::mat4 CalculateTransformMatrix(GameObject* gameObject);
};