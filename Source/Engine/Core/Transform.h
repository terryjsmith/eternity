
#ifndef transform_h
#define transform_h

#include <eternity.h>
#include <Core/MathTypes.h>
#include <Core/GigaObject.h>

GIGA_CLASS()
class GIGA_API Transform : public GigaObject {
public:
	Transform();
	~Transform() = default;

	GIGA_CLASS_NAME("Transform");

    GIGA_FUNCTION() void SetParent(Transform* transform);
    GIGA_FUNCTION() vector3 GetLocalPosition() { return m_position; }
    
    GIGA_FUNCTION() vector3 GetWorldPosition();

    GIGA_FUNCTION() void SetLocalPosition(vector3 position);
    GIGA_FUNCTION() void SetWorldPosition(vector3 position);

    GIGA_FUNCTION() vector3 GetLocalScaling() { return m_scaling; }
    GIGA_FUNCTION() vector3 GetWorldScaling();

	GIGA_FUNCTION() void SetLocalScaling(vector3 scaling);
	GIGA_FUNCTION() void SetWorldScaling(vector3 scaling);

	GIGA_FUNCTION() quaternion GetLocalRotation() { return m_rotation; }
	GIGA_FUNCTION() quaternion GetWorldRotation();

	GIGA_FUNCTION() void SetLocalRotation(quaternion rotation);
	GIGA_FUNCTION() void SetWorldRotation(quaternion rotation);

	GIGA_FUNCTION() vector3 GetUp();
	GIGA_FUNCTION() vector3 GetRight();
	GIGA_FUNCTION() vector3 GetForward();

	GIGA_FUNCTION() void Move(vector3 amount);
	GIGA_FUNCTION() void Rotate(vector3 axis, float degrees);
	GIGA_FUNCTION() void Scale(vector3 amount);

	matrix4 GetMatrix();

	GIGA_FUNCTION() void SetRoot(bool root) { m_root = root; }

protected:
	Transform* m_parent;

	// Whether updating this should also update the root
	bool m_root;

	vector3 m_position;
	vector3 m_scaling;
	quaternion m_rotation;
};

#endif
