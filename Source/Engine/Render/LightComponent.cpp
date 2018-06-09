
#include <Render/LightComponent.h>

LightComponent::LightComponent() {
    m_color = vector3(1.0f, 1.0f, 1.0f);
    m_attenuation = 10.0f;
    m_passes = 1;
    m_depthPass = 0;
    m_camera = 0;
    m_depthTexture = 0;
}
