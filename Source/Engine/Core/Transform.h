
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

	GIGA_FUNCTION(Scriptable) vector3 GetLocalPosition() { return m_position; }
	GIGA_FUNCTION(Scriptable) vector3 GetWorldPosition();

	GIGA_FUNCTION(Scriptable) void SetLocalPosition(vector3 position);
	GIGA_FUNCTION(Scriptable) void SetWorldPosition(vector3 position);

	GIGA_FUNCTION(Scriptable) vector3 GetLocalScaling() { return m_scaling; }
	GIGA_FUNCTION(Scriptable) vector3 GetWorldScaling();

	GIGA_FUNCTION(Scriptable) void SetLocalScaling(vector3 scaling);
	GIGA_FUNCTION(Scriptable) void SetWorldScaling(vector3 scaling);

	GIGA_FUNCTION(Scriptable) quaternion GetLocalRotation() { return m_rotation; }
	GIGA_FUNCTION(Scriptable) quaternion GetWorldRotation();

	GIGA_FUNCTION(Scriptable) void SetLocalRotation(quaternion rotation);
	GIGA_FUNCTION(Scriptable) void SetWorldRotation(quaternion rotation);

	GIGA_FUNCTION(Scriptable) vector3 GetUp();
	GIGA_FUNCTION(Scriptable) vector3 GetRight();
	GIGA_FUNCTION(Scriptable) vector3 GetForward();

	GIGA_FUNCTION(Scriptable) void Move(vector3 amount);
	GIGA_FUNCTION(Scriptable) void Rotate(vector3 axis, float degrees);
	GIGA_FUNCTION(Scriptable) void Scale(vector3 amount);

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