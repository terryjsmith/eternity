
#include <Render/CameraComponent.h>
#include <glm/gtc/matrix_transform.hpp>

CameraComponent::CameraComponent() {
	m_fnear = 0.1f;
	m_ffar = 100.0f;
    m_fov = 45.0f;
}

matrix4 CameraComponent::GetViewMatrix() {
	Transform* transform = GetTransform();
	quaternion rotation = transform->GetWorldRotation();
	vector3 position = transform->GetWorldPosition();

    vector3 look = transform->GetLook();
    vector3 up = transform->GetUp();

	return(glm::lookAt(position, position + look, up));
}

matrix4 CameraComponent::GetProjectionMatrix() {
	return(glm::perspective(glm::radians(m_fov), m_aspect, m_fnear, m_ffar));
}
