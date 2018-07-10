
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
    GIGA_CLASS_BODY();

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
	GIGA_FUNCTION() vector3 GetLook();
    
    GIGA_FUNCTION() void SetUp(vector3 up);
    GIGA_FUNCTION() void SetRight(vector3 right);
    GIGA_FUNCTION() void SetLook(vector3 look);

	GIGA_FUNCTION() void Move(vector3 amount);
	GIGA_FUNCTION() void Rotate(vector3 axis, float degrees);
	GIGA_FUNCTION() void Scale(vector3 amount);

	matrix4 GetMatrix();
    
    void AddChild(Transform* transform);
    void RemoveChild(Transform* transform);
    
	GIGA_FUNCTION() void SetRoot(bool root) { m_root = root; }

protected:
	Transform* m_parent;
    std::vector<Transform*> m_children;

	// Whether updating this via Move/Rotate/Scale should also update the root
	GIGA_VARIABLE(Serialize) bool m_root;

	GIGA_VARIABLE(Serialize) vector3 m_position;
	GIGA_VARIABLE(Serialize) vector3 m_scaling;
	GIGA_VARIABLE(Serialize) quaternion m_rotation;
    
    // Custom look vectors
    vector3 m_up;
    vector3 m_right;
    vector3 m_look;
};

#endif
