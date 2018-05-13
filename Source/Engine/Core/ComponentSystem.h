
#ifndef componentsystem_h
#define componentsystem_h

#include <eternity.h>
#include <Core/System.h>

template<class T>
class GIGA_API ComponentSystem : public System {
public:
	T * CreateComponent() {
		T obj;
		m_components.push_back(obj);

		return(&m_components.back());
	}

protected:
	std::vector<T> m_components;
};

#endif