
#include <Core/GigaObject.h>
#include <Core/MetaSystem.h>
#include <Core/Application.h>

void GigaObject::LockMutex() {
	m_mutex->lock();
}

bool GigaObject::TryLockMutex() {
	return(m_mutex->try_lock());
}

void GigaObject::UnlockMutex() {
	m_mutex->unlock();
}

Variant* GigaObject::Call(std::string func, int argc, Variant** argv) {
	MetaSystem* metaSystem = GetSystem<MetaSystem>();

	std::string className = this->GetGigaName();
	CallableFunction c = metaSystem->FindFunction(className, func);

	GIGA_ASSERT(c != 0, "Callable function not found.");

	return(c(this, argc, argv));
}

bool GigaObject::HasFunction(std::string func) {
	MetaSystem* metaSystem = GetSystem<MetaSystem>();

	std::string className = this->GetGigaName();
	CallableFunction c = metaSystem->FindFunction(className, func);

	return(c != 0);
}

std::string GigaObject::ToString() {
	return(this->GetGigaName() + "[]");
}