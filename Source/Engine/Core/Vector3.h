
#ifndef vector3_h
#define vector3_h

#include <glm/glm.hpp>
#include <Core/GigaObject.h>

class Vector3 : public GigaObject {
public:
	Vector3() : m_vector(0.0f) { }
	Vector3(float x, float y, float z) : m_vector(x, y, z) { }
	Vector3(const Vector3 &obj) : m_vector(obj.m_vector.x, obj.m_vector.y, obj.m_vector.z) { }
	~Vector3() = default;

	GIGA_CLASS_NAME("Vector3");

	const glm::vec3& operator *() const {
		return m_vector;
	}

	Vector3& operator=(const Vector3& other) {
		m_vector.x = other.m_vector.x;
		m_vector.y = other.m_vector.y;
		m_vector.z = other.m_vector.z;
        return(vec);
	}

	Vector3 operator-(const Vector3& other) {
		Vector3 vec;
		vec.m_vector.x = m_vector.x - other.m_vector.x;
		vec.m_vector.y = m_vector.y - other.m_vector.y;
		vec.m_vector.z = m_vector.z - other.m_vector.z;
		return(vec);
	}

	Vector3 operator+(const Vector3& other) {
		Vector3 vec;
		vec.m_vector.x = m_vector.x + other.m_vector.x;
		vec.m_vector.y = m_vector.y + other.m_vector.y;
		vec.m_vector.z = m_vector.z + other.m_vector.z;
		return(vec);
	}

	Vector3 operator*(const Vector3& other) {
		Vector3 vec;
		vec.m_vector.x = m_vector.x * other.m_vector.x;
		vec.m_vector.y = m_vector.y * other.m_vector.y;
		vec.m_vector.z = m_vector.z * other.m_vector.z;
		return(vec);
	}

	Vector3 operator/(const Vector3& other) {
		Vector3 vec;
		vec.m_vector.x = m_vector.x / other.m_vector.x;
		vec.m_vector.y = m_vector.y / other.m_vector.y;
		vec.m_vector.z = m_vector.z / other.m_vector.z;
		return(vec);
	}

protected:
	glm::vec3 m_vector;
};

#endif
