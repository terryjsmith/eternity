
#ifndef scriptvariant_h
#define scriptvariant_h

#include <eternity.h>
#include <v8.h>
#include <Core/Variant.h>

/**
* Extension of our base variant class to integrate with V8's types
*/
class GIGA_API ScriptVariant : public Variant {
public:
	// JS types
	enum JSType {
		VAR_FUNCTION = Variant::Type::VAR_LASTTYPE
	};

	/**
	* Constructor
	*/
	ScriptVariant(v8::Local<v8::Value> value);

	/**
	* Set operators
	*/
	Variant& operator =(v8::Local<v8::Value> rhs);

	/**
	* Get value in V8 friendly format
	*/
	v8::Local<v8::Value> GetValue();

	/**
	* Checker
	*/
	bool IsFunction() { return(m_type == VAR_FUNCTION); }

};

#endif