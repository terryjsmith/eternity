
#include <Core/Timer.h>
#include <Core/TimeSystem.h>

Timer::Timer() {
	memset(&m_startTime, 0, sizeof(timespec));
	memset(&m_lastTime, 0, sizeof(timespec));
}

void Timer::Start() {
	TimeSystem::GetTimestamp(&m_startTime);
}

float Timer::Duration() {
	timespec ts;
	TimeSystem::GetTimestamp(&ts);

	timespec delta = TimeSystem::Diff(&m_startTime, &ts);

	return(delta.tv_sec + (delta.tv_nsec / 1000000000.0f));
}

float Timer::Reset() {
	timespec ts;
	TimeSystem::GetTimestamp(&ts);

	timespec delta = TimeSystem::Diff(&m_startTime, &ts);

	m_startTime = ts;
	return(delta.tv_sec + (delta.tv_nsec / 1000000000.0f));
}

void Timer::End() {
	memset(&m_startTime, 0, sizeof(timespec));
}