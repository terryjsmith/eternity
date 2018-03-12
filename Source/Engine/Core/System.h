
#ifndef system_h
#define system_h

#include <eternity.h>

class GIGA_API System {
public:
	System() = default;
	virtual ~System() = default;

	virtual void Initialize() { }
	virtual void Update(float delta) { }
	virtual void Shutdown() { }
};

#endif