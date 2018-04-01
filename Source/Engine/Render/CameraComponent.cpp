
#include <Render/CameraComponent.h>
#include <glm/gtc/matrix_transform.hpp>

CameraComponent::CameraComponent() {
	m_fnear = 0.1f;
	m_ffar = 100.0f;
    m_fov = 45.0f;
}

void CameraComponent::SetViewport(int windowWidth, int windowHeight) {
	m_aspect = (float)windowWidth / (float)windowHeight;
}

matrix4 CameraComponent::GetViewMatrix() {
	Transform* transform = GetTransform();
	quaternion rotation = transform->GetWorldRotation();
	Vector3 position = transform->GetWorldPosition();

	Vector3 look = rotation * Vector3(0.0f, 0.0f, -1.0f);
	Vector3 up = rotation * Vector3(0, 1, 0);

	return(glm::lookAt(position, position + look, up));
}

matrix4 CameraComponent::GetProjectionMatrix() {
	return(glm::perspective(glm::radians(m_fov), m_aspect, m_fnear, m_ffar));
}
