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
    glm::mat4 translationMatrix = glm::translate(glm::mat4(1.0f), gameObject->transform.position);
    glm::mat4 rotationMatrix = glm::eulerAngleXYZ(
        glm::radians(gameObject->transform.rotation.x),
        glm::radians(gameObject->transform.rotation.y),
        glm::radians(gameObject->transform.rotation.z)
    );
    glm::mat4 scaleMatrix = glm::scale(glm::mat4(1.0f), gameObject->transform.scale);
    return translationMatrix * rotationMatrix * scaleMatrix;
}

glm::mat4 Camera::CalculateViewMatrix() {
	glm::vec3 direction;
	direction.x = cos(glm::radians(this->transform.rotation.x)) * cos(glm::radians(this->transform.rotation.y));
	direction.y = sin(glm::radians(this->transform.rotation.y));
	direction.z = sin(glm::radians(this->transform.rotation.x)) * cos(glm::radians(this->transform.rotation.y));
	direction = glm::normalize(direction);
	return glm::lookAt(this->transform.position, this->transform.position + direction, glm::vec3(0.0f, 1.0f, 0.0f));
}

void Camera::Render(std::vector<RenderData> data) {
	if(old_viewport_pos != viewport_pos) {
		old_viewport_pos = viewport_pos;
		projection = glm::perspective(
		glm::radians(90.0f), // Field of view in radians
		viewport_pos.z / viewport_pos.w,
		0.1f, 1000.0f
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
	Renderer::StartRender();
	for (RenderData d : data) {
		Renderer::Render(projection, view, d);
	}
	Renderer::FinishRender();
}
