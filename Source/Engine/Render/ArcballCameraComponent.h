
#ifndef arccameracomponent_h
#define arccameracomponent_h

#include <eternity.h>
#include <Render/CameraComponent.h>

/**
* Our arc camera (follow the player) camera
*/
class GIGA_API ArcballCameraComponent : public CameraComponent {
public:
	ArcballCameraComponent();
	~ArcballCameraComponent() = default;

public:
	/**
	* Rotate the camera around the y-axis
	*/
	void RotateY(float degrees);

	/**
	* Rotate the camera around the x-axis
	*/
	void RotateX(float degrees);

	/**
	* Set the min/max distance from the player
	*/
	void SetDistance(float fmin, float fmax);

	/**
	* Set target to a point in space instead of a mesh
	*/
	void SetTarget(vector3 point);

protected:
	float m_minDistance, m_maxDistance;
	float m_currentDistance;

	float m_minPitch, m_maxPitch;
	float m_currentPitch;

	vector3 m_targetPosition;
};

#endif