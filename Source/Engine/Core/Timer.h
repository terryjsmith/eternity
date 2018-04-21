
#ifndef timer_h
#define timer_h

#include <eternity.h>
#include <Core/GigaObject.h>

GIGA_CLASS()
class GIGA_API Timer : public GigaObject {
public:
	Timer();
	~Timer() = default;

	GIGA_CLASS_NAME("Timer");

	/**
	* Start, get difference, reset and end timer
	*/
	GIGA_FUNCTION() void Start();
	GIGA_FUNCTION() float Duration();
	GIGA_FUNCTION() float Reset();
	GIGA_FUNCTION() void End();

protected:
	// Start time of timer
	timespec m_startTime;
	timespec m_lastTime;
};

#endif