
#ifndef rendercomponent_h
#define rendercomponent_h

#include <eternity.h>
#include <Core/Transform.h>
#include <Core/Component.h>

class GIGA_API RenderComponent : public Component {
public:
	RenderComponent();
	virtual ~RenderComponent();

	GIGA_FUNCTION() Transform* GetTransform() { return m_transform; }
    
    /**
     * Overridable notifications
     */
    void OnEntityAssigned();
    void OnComponentAdded(Component* component);

protected:
	 Transform* m_transform;
};

#endif
