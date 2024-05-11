#include <util/SceneUtils.hpp>

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/euler_angles.hpp>

void Scene::Render() {
	for (GameObject gameObject : this->main) {
		RenderGameObject(&gameObject, glm::mat4(1.0f)); // Start with an identity matrix for the root object
	}
}

void Scene::RenderGameObject(GameObject* gameObject, glm::mat4 parentMatrix) {
	// Calculate the model matrix for the current game object
	glm::mat4 modelMatrix = parentMatrix * CalculateTransformMatrix(gameObject);

	// Check if the gameObject has a parent
	if (gameObject->parent != nullptr) {
		// Get the parent matrix from the parent gameObject
		glm::mat4 parentMatrix = GetParentMatrix(gameObject->parent);
		modelMatrix = parentMatrix * CalculateTransformMatrix(gameObject);
	}

	// Render the current game object using the Renderer
	Renderer::Render(modelMatrix, &gameObject->render_data.main_shader, &gameObject->render_data.object_mesh);

	// Recursively render children of the current game object
	for (GameObject child : gameObject->children) {
		RenderGameObject(&child, modelMatrix);
	}
}

glm::mat4 Scene::CalculateTransformMatrix(GameObject* gameObject) {

	glm::mat4 translationMatrix = glm::translate(glm::mat4(1.0f), gameObject->transform.position);
	translationMatrix = glm::rotate(translationMatrix,  gameObject->transform.rotation.x, glm::vec3(1.0f, 0.0f, 0.0f));
	translationMatrix = glm::rotate(translationMatrix,  gameObject->transform.rotation.y, glm::vec3(0.0f, 1.0f, 0.0f));
	translationMatrix = glm::rotate(translationMatrix,  gameObject->transform.rotation.z, glm::vec3(0.0f, 0.0f, 1.0f));
	translationMatrix = glm::scale(translationMatrix, gameObject->transform.scale);

	return translationMatrix;
}

glm::mat4 Scene::GetParentMatrix(GameObject* parent) {
	// Check if the parent has a parent
	if (parent->parent != nullptr) {
		// Get the parent matrix from the parent's parent
		glm::mat4 parentMatrix = GetParentMatrix(parent->parent);
		return parentMatrix * CalculateTransformMatrix(parent);
	} else {
		// No parent, return the identity matrix
		return glm::mat4(1.0f);
	}
}