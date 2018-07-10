
#ifndef transformcomponent_h
#define transformcomponent_h

#include <eternity.h>
#include <Core/Component.h>
#include <Core/Transform.h>

GIGA_CLASS()
class GIGA_API TransformComponent : public Component {
public:
    TransformComponent();
    ~TransformComponent();
    
    GIGA_CLASS_NAME("TransformComponent");
    GIGA_CLASS_BODY();
    
    GIGA_FUNCTION() Transform* GetTransform() { return m_transform; }
    
    /**
     * After deserialization
     */
    void PostDeserialize();
    
protected:
    GIGA_VARIABLE(Serialize) Transform* m_transform;
};

#endif
