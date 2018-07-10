
#ifndef gigaobject_h
#define gigaobject_h

#include <eternity.h>

class GigaObject;
class Variant;
class DataRecord;

#define GIGA_CLASS_NAME(str) virtual std::string GetGigaName() { return str; } 

typedef Variant*(*CallableFunction)(GigaObject* obj, int argc, Variant** argv);

/**
* Base object class for all other objects in the system
*/
class GIGA_API GigaObject {
public:
	GigaObject();
	virtual ~GigaObject() = default;

	/**
	 * Pure virtual function that specifies string version of class name
	 * Use GIGA_CLASS_NAME("name") to set
	 */
	virtual std::string GetGigaName() = 0;

	/**
	 * Call a function on this object by passing a variable number of variants
	 */
	Variant* Call(std::string func, int argc, Variant** argv);

	/**
	 * Check for the existence of a callable function
	 */
	bool HasFunction(std::string func);

	/**
	* Lock this object to the thread (or try to)
	*/
	void LockMutex();
	bool TryLockMutex();

	/**
	* Unlock
	*/
	void UnlockMutex();
    
    /**
     * Serialize/deserialize from data record
     */
    virtual void Serialize(DataRecord* record) { }
    virtual void Deserialize(DataRecord* record) { }
    
    /**
     * Hooks after de/serializing
     */
    virtual void PreSerialize() { }
    virtual void PostSerialize() { }
    
    virtual void PreDeserialize() { }
    virtual void PostDeserialize() { }

protected:
	// Lockable mutex (multi-threading)
    std::mutex* m_mutex;
};

#endif
