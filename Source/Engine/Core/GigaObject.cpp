
#include <Core/GigaObject.h>

void GigaObject::LockMutex() {
	m_mutex->lock();
}

bool GigaObject::TryLockMutex() {
	return(m_mutex->try_lock());
}

void GigaObject::UnlockMutex() {
	m_mutex->unlock();
}