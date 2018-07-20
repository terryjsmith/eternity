
#include <Render/ArcballCameraComponent.h>

ArcballCameraComponent::ArcballCameraComponent() {
	m_minDistance = 5.0f;
	m_maxDistance = 15.0f;
	m_currentDistance = 10.0f;
	m_minPitch = glm::radians(10.0f);
	m_maxPitch = glm::radians(30.0f);
	m_currentPitch = glm::radians(20.0f);
}

void ArcballCameraComponent::RotateY(float degrees) {
	GetTransform()->Rotate(vector3(0, 1, 0), degrees);
	vector3 look = GetTransform()->GetLook();

	vector3 position = m_targetPosition - (m_currentDistance * look);
	GetTransform()->SetWorldPosition(position);
}

void ArcballCameraComponent::RotateX(float degrees) {
	vector3 right = GetTransform()->GetRight();

	GetTransform()->Rotate(right, degrees);
	vector3 look = GetTransform()->GetLook();

	vector3 position = m_targetPosition - (m_currentDistance * look);
	GetTransform()->SetWorldPosition(position);
}

void ArcballCameraComponent::SetTarget(vector3 point) {
	// Save the position of the mesh
	m_targetPosition = point;

	// Figure out our current pitch
	vector3 position = GetTransform()->GetWorldPosition();
	vector3 look = glm::normalize(m_targetPosition - position);
	m_currentPitch = asin(look.y);

	// Set distance
	vector3 diff = position - m_targetPosition;
	m_currentDistance = glm::length(diff);

	// Set the rotation
        quaternion q;
        float d = glm::dot(look, vector3(0, 0, -1));
	if ((d < 1.0f) && (d > -1.0f)) {
            float s = std::sqrt((1 + d) * 2);
            float invs = 1 / s;

            vector3 c = glm::cross(vector3(0, 0, -1), look);

            q.x = c.x * invs;
            q.y = c.y * invs;
            q.z = c.z * invs;
            q.w = s * 0.5f;
            q = glm::normalize(q);
        }
	GetTransform()->SetWorldRotation(q);

	// Then retreat from the player position in normalized look * distance
	vector3 lookN = glm::normalize(look);
	GetTransform()->SetWorldPosition(m_targetPosition - (m_currentDistance * lookN));
}

void ArcballCameraComponent::SetDistance(float fmin, float fmax) {
	m_minDistance = fmin;
	m_maxDistance = fmax;
}
