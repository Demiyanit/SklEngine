#pragma once

#include <vector>
#include <string>
#include <any>
#include <renderer/Renderer.hpp>

class GameObject;

class Transform {
public:
	GameObject* obj;
	glm::vec3 position;
	glm::vec3 rotation;
	glm::vec3 scale;
};

class Component {
public:
	std::string name;
	//The game object this transform is attached to
	GameObject* obj;
	//Same here
	Transform* tr;
	std::any data;
};

class GameObject {
public:
	GameObject* parent;
	RenderData render_data;
	Transform transform;
	std::string name;
	std::vector<GameObject> children;
	std::vector<Component> components;
};

class Scene {
public:
	std::string name;
	void Render();
	glm::mat4 GetParentMatrix(GameObject* parent);
	void RenderGameObject(GameObject* gameObject, glm::mat4 parentMatrix);
	glm::mat4 CalculateTransformMatrix(GameObject* gameObject);
	std::vector<GameObject> main;
};