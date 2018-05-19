
#include <Core/Transform.h>
#include <glm/gtx/transform.hpp>

Transform::Transform() {
	m_position = vector3(0, 0, 0);
	m_scaling = vector3(1, 1, 1);
	m_rotation = quaternion(1, 0, 0, 0);
    
    m_parent = 0;
}

void Transform::SetParent(Transform* transform) {
	// First, whether we have a current parent or not, set everything to world positions
	m_position = GetWorldPosition();
	m_scaling = GetWorldScaling();
	m_rotation = GetWorldRotation();

	if (transform) {
		m_position = m_position - transform->GetWorldPosition();
		m_scaling = m_position - transform->GetWorldScaling();
		m_rotation = glm::inverse(transform->GetWorldRotation()) * m_rotation;
	}
}

vector3 Transform::GetWorldPosition() {
	vector3 sum = m_position;
	if (m_parent) {
		sum += m_parent->GetWorldPosition();
	}

	return(sum);
}

vector3 Transform::GetWorldScaling() {
	vector3 sum = m_scaling;
	if (m_parent) {
		sum += m_parent->GetWorldScaling();
	}

	return(sum);
}

quaternion Transform::GetWorldRotation() {
	quaternion sum = m_rotation;
	if (m_parent) {
		sum *= m_parent->GetWorldRotation();
	}

	return(sum);
}

void Transform::SetLocalPosition(vector3 position) {
	if (m_root && m_parent) {
		m_parent->SetLocalPosition(position);
		return;
	}

	m_position = position;
}

void Transform::SetWorldPosition(vector3 position) {
	if (m_root && m_parent) {
		m_parent->SetWorldPosition(position);
		return;
	}

	m_position = position;

	if (m_parent) {
		vector3 other = m_parent->GetWorldPosition();
		m_position = m_position - other;
	}
}

void Transform::SetLocalScaling(vector3 scaling) {
	if (m_root && m_parent) {
		m_parent->SetLocalScaling(scaling);
		return;
	}

	m_scaling = scaling;
}

void Transform::SetWorldScaling(vector3 scaling) {
	if (m_root && m_parent) {
		m_parent->SetWorldScaling(scaling);
		return;
	}

	m_scaling = scaling;

	if (m_parent) {
		vector3 other = m_parent->GetWorldScaling();
		m_scaling = m_scaling - other;
	}
}

void Transform::SetLocalRotation(quaternion rotation) {
	if (m_root && m_parent) {
		m_parent->SetLocalRotation(rotation);
		return;
	}

	m_rotation = rotation;
}

void Transform::SetWorldRotation(quaternion rotation) {
	if (m_root && m_parent) {
		m_parent->SetWorldRotation(rotation);
		return;
	}

	m_rotation = rotation;

	if (m_parent) {
		m_rotation = glm::inverse(m_parent->GetWorldRotation()) * m_rotation;
	}
}

matrix4 Transform::GetMatrix() {
	matrix4 t = glm::translate(glm::mat4(1.0), GetWorldPosition());
	matrix4 s = glm::scale(glm::mat4(1.0), GetWorldScaling());
	matrix4 r = glm::mat4_cast(GetWorldRotation());

	matrix4 modelMatrix = t * r * s;

	return(modelMatrix);
}

vector3 Transform::GetUp() {
	return(m_rotation * vector3(0, 1, 0));
}

vector3 Transform::GetRight() {
	return(m_rotation * vector3(1, 0, 0));
}

vector3 Transform::GetLook() {
	return(m_rotation * vector3(0, 0, -1));
}

void Transform::Move(vector3 amount) {
	if (m_root && m_parent) {
		m_parent->Move(amount);
		return;
	}

	m_position += amount;
}

void Transform::Rotate(vector3 axis, float degrees) {
	if (m_root && m_parent) {
		m_parent->Rotate(axis, degrees);
		return;
	}

	quaternion delta = glm::angleAxis(glm::radians(degrees), axis);
	m_rotation = m_rotation * delta;
}

void Transform::Scale(vector3 amount) {
	if (m_root && m_parent) {
		m_parent->Scale(amount);
		return;
	}

	m_scaling *= amount;
}
