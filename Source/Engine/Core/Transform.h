
#ifndef transform_h
#define transform_h

#include <eternity.h>
#include <Core/MathTypes.h>
#include <Core/GigaObject.h>

GIGA_CLASS(Scriptable)
class GIGA_API Transform : public GigaObject {
public:
	Transform();
	~Transform() = default;

	GIGA_CLASS_NAME("Transform");

	void SetParent(Transform* transform);

	vector3 GetLocalPosition() { return m_position; }
	vector3 GetWorldPosition();

	void SetLocalPosition(vector3 position);
	void SetWorldPosition(vector3 position);

	vector3 GetLocalScaling() { return m_scaling; }
	vector3 GetWorldScaling();

	void SetLocalScaling(vector3 scaling);
	void SetWorldScaling(vector3 scaling);

	quaternion GetLocalRotation() { return m_rotation; }
	quaternion GetWorldRotation();

	void SetLocalRotation(quaternion rotation);
	void SetWorldRotation(quaternion rotation);

	vector3 GetUp();
	vector3 GetRight();
	vector3 GetForward();

	void Move(vector3 amount);
	void Rotate(vector3 axis, float degrees);
	void Scale(vector3 amount);

	matrix4 GetMatrix();

	void SetRoot(bool root) { m_root = root; }

protected:
	Transform* m_parent;

	// Whether updating this should also update the root
	bool m_root;

	vector3 m_position;
	vector3 m_scaling;
	quaternion m_rotation;
};

#endif