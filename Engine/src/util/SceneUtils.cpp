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
    return rotationMatrix  * scaleMatrix * translationMatrix;
}

glm::mat4 Camera::CalculateViewMatrix() {
    glm::mat4 rotationMatrix = glm::eulerAngleXYZ(
      glm::radians(this->transform.rotation.x),
      glm::radians(this->transform.rotation.y),
      glm::radians(this->transform.rotation.z)
    );

    glm::mat4 translationMatrix = glm::translate(glm::mat4(1.0f), -this->transform.position);

    glm::mat4 viewMatrix = rotationMatrix * translationMatrix;

    return viewMatrix;
}
void Camera::Render(std::vector<RenderData> data) {
	if(old_viewport_pos != viewport_pos) {
		old_viewport_pos = viewport_pos;
		projection = glm::perspective(
		glm::radians(44.0f), // Field of view in radians
		viewport_pos.z / viewport_pos.w,
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
	Renderer::StartRender();
	for (RenderData d : data) {
		Renderer::Render(projection, view, d);
	}
	Renderer::FinishRender();
}
