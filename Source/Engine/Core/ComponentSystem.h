
#ifndef componentsystem_h
#define componentsystem_h

#include <eternity.h>
#include <Core/System.h>
#include <Core/Component.h>

class ComponentSystemBase : public System {
public:
    ComponentSystemBase() = default;
    ~ComponentSystemBase() = default;
    
    virtual void AddComponent(Component* component) { }

	virtual void Clear() { }
};

template<class T>
class GIGA_API ComponentSystem : public ComponentSystemBase {
public:
	void AddComponent(Component* component) {
        T* obj = dynamic_cast<T*>(component);
        if(obj) {
            m_components.push_back(obj);
        }
    }

	virtual void Clear() { m_components.clear(); }

protected:
	std::vector<T*> m_components;
};

#endif
