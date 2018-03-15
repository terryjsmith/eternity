
#ifndef rendercomponent_h
#define rendercomponent_h

#include <eternity.h>
#include <Core/Transform.h>
#include <Core/Component.h>

GIGA_CLASS(Scriptable)
class GIGA_API RenderComponent : public Component {
public:
	RenderComponent();
	virtual ~RenderComponent() = default;

	Transform* GetTransform() { return m_transform; }

protected:
	Transform* m_transform;
};

#endif