
#ifndef gigaobject_h
#define gigaobject_h

#include <eternity.h>

#define GIGA_CLASS_NAME(str) virtual std::string GetGigaName() { return str; } 

/**
* Base object class for all other objects in the system
*/
class GIGA_API GigaObject {
public:
	GigaObject() = default;
	~GigaObject() = default;

	/**
	 * Pure virtual function that specifies string version of class name
	 * Use GIGA_CLASS_NAME("name") to set
	 */
	virtual std::string GetGigaName() = 0;

	/**
	* Lock this object to the thread (or try to)
	*/
	void LockMutex();
	bool TryLockMutex();

	/**
	* Unlock
	*/
	void UnlockMutex();

protected:
	// Lockable mutex (multi-threading)
	std::unique_ptr<std::mutex> m_mutex;
};

#endif