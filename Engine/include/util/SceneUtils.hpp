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
	std::string tag;
	//The game object this transform is attached to
	GameObject* parent;
	//Same here
	Transform* tr;
	std::any data;
	bool Callable;
	bool Renderable;
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

class Camera : Component {
public:
	Transform transform;
	glm::vec4 clear_color;
	glm::vec4 viewport_pos;
	glm::vec4 old_viewport_pos;
	glm::mat4 projection;
	glm::mat4 view;
	glm::mat4 CalculateViewMatrix();
	glm::mat4 CalculateCurrentMatrix();
	void Render(std::vector<RenderData> data);
};

class Scene {
public:
	std::string name;
	std::vector<RenderData> ConstructRenderData();
	void RenderGameObject(GameObject* gameObject, glm::mat4 parentMatrix, std::vector<RenderData>& renderData);
	glm::mat4 CalculateTransformMatrix(GameObject* gameObject);
	std::vector<GameObject> main;
	std::vector<Component> scene_components;
};