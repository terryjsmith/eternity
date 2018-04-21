
#include <Core/TimeSystem.h>

#ifdef __MACH__
#include <mach/clock.h>
#include <mach/mach.h>
#endif

#ifdef WIN32

#define CLOCK_REALTIME	0

extern "C" { FILE __iob_func[3] = { *stdin,*stdout,*stderr }; }

int clock_gettime(int, struct timespec *spec) {
    __int64 wintime;
    
    GetSystemTimeAsFileTime((FILETIME*)&wintime);
    
    //1 jan 1601 to 1 jan 1970
    wintime -= 116444736000000000i64;
    spec->tv_sec = wintime / 10000000i64;           //seconds
    spec->tv_nsec = wintime % 10000000i64 * 100;    //nano-seconds
    
    return 0;
}

#endif

void TimeSystem::GetTimestamp(struct timespec *ts) {
#ifdef __MACH__ // OS X does not have clock_gettime, use clock_get_time
    clock_serv_t cclock;
    mach_timespec_t mts;
    host_get_clock_service(mach_host_self(), CALENDAR_CLOCK, &cclock);
    clock_get_time(cclock, &mts);
    mach_port_deallocate(mach_task_self(), cclock);
    ts->tv_sec = mts.tv_sec;
    ts->tv_nsec = mts.tv_nsec;
#else
    clock_gettime(CLOCK_REALTIME, ts);
#endif
}

TimeSystem::TimeSystem() {
    memset(&m_startupTime, 0, sizeof(timespec));
    GetTimestamp(&m_startupTime);
    
    m_overrideTick = 0;
}

timespec TimeSystem::Diff(timespec* start, timespec* end) {
    timespec t;
    
    if(end->tv_nsec - start->tv_nsec < 0) {
        t.tv_sec = end->tv_sec - start->tv_sec - 1;
        t.tv_nsec = end->tv_nsec - start->tv_nsec + 1000000000.0f;
    }
    else {
        t.tv_sec = end->tv_sec - start->tv_sec;
        t.tv_nsec = end->tv_nsec - start->tv_nsec;
    }
    return(t);
}

void TimeSystem::Sleep(int milliseconds) {
#ifdef WIN32
    ::Sleep(milliseconds);
#else
    usleep(milliseconds * 1000);
#endif
}

int TimeSystem::GetCurrentTick() {
    if (m_overrideTick) {
        return(m_overrideTick);
    }
    
    timespec t;
    GetTimestamp(&t);
    
    timespec diff = TimeSystem::Diff(&m_startupTime, &t);
    double d = (double)diff.tv_sec + ((double)diff.tv_nsec / 1000000000);
    
    return(floor(d * GIGA_TICKS_PER_SECOND));
}

float TimeSystem::GetCurrentTickTime() {
    timespec t;
    TimeSystem::GetTimestamp(&t);
    
    timespec diff = TimeSystem::Diff(&m_startupTime, &t);
    double d = (double)diff.tv_sec + ((double)diff.tv_nsec / 1000000000);
    
    return(d);
}
