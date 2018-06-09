
#ifndef camera_h
#define camera_h

#include <eternity.h>
#include <Core/MathTypes.h>
#include <Render/RenderComponent.h>

GIGA_CLASS()
class GIGA_API CameraComponent : public RenderComponent {
public:
	CameraComponent();
	~CameraComponent() = default;

	GIGA_CLASS_NAME("CameraComponent");

    /**
     * Get/set near/far/aspect/FOV
     */
    void SetFar(float ffar) { m_ffar = ffar; }
    void SetNear(float fnear) { m_fnear = fnear; }
    void SetAspectRatio(float aspect) { m_aspect = aspect; }
    void SetFOV(float fov) { m_fov = fov; }
    
    /**
     * Get camera matrices
     */
	matrix4 GetViewMatrix();
	matrix4 GetProjectionMatrix();

protected:
	float m_fnear, m_ffar;
	float m_fov;
	float m_aspect;
};

#endif
