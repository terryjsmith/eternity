
#ifndef camera_h
#define camera_h

#include <eternity.h>
#include <Core/MathTypes.h>
#include <Render/RenderComponent.h>

class GIGA_API CameraComponent : public RenderComponent {
public:
	CameraComponent();
	~CameraComponent() = default;

	GIGA_CLASS_NAME("CameraComponent");

	void SetViewport(int windowWidth, int windowHeight);

	matrix4 GetViewMatrix();
	matrix4 GetProjectionMatrix();

protected:
	float m_fnear, m_ffar;
	float m_fov;
	float m_aspect;
};

#endif