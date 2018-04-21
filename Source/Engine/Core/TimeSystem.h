
#ifndef timesystem_h
#define timesystem_h

#include <eternity.h>
#include <Core/System.h>

GIGA_CLASS(Singleton)
class GIGA_API TimeSystem : public System {
public:
    TimeSystem();
    ~TimeSystem() = default;
    
    GIGA_CLASS_NAME("TimeSystem");
    
    /**
     * Get current timestamp (UTC)
     */
    GIGA_FUNCTION() static void GetTimestamp(struct timespec *ts);
    
    /**
     * Get difference between two timespecs
     */
    GIGA_FUNCTION() static timespec Diff(timespec* start, timespec* end);
    
    /**
     * Sleep
     */
    GIGA_FUNCTION() static void Sleep(int milliseconds);
    
    /**
     * Get current tick
     */
    GIGA_FUNCTION() int GetCurrentTick();
    
    /**
     * Get current offset time since start
     */
    float GetCurrentTickTime();
    
    /**
     * Get system startup time
     */
    timespec GetStartupTime() { return m_startupTime; }
    
protected:
    // Startup time of the system
    timespec m_startupTime;
    
    // Manual tick offset for replaying
    int m_overrideTick;
};

#endif
