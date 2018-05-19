
#include <Scripting/ScriptVariant.h>
#include <Scripting/ScriptThread.h>
#include <Scripting/Types/Vector3.h>

ScriptVariant::ScriptVariant(v8::Local<v8::Value> value) {
	*this = value;
}

Variant& ScriptVariant::operator =(v8::Local<v8::Value> rhs) {
	v8::Isolate* isolate = v8::Isolate::GetCurrent();
	v8::String::Utf8Value type(rhs->TypeOf(isolate));

	if (rhs->IsInt32() || rhs->IsUint32()) {
		m_type = VAR_INT32;
		m_data.i32 = rhs->NumberValue();
		return *this;
	}

	if (rhs->IsNumber() && rhs->IsInt32() == false && rhs->IsUint32() == false) {
		m_type = VAR_FLOAT;
		m_data.f1 = rhs->NumberValue();
		return *this;
	}

	if (rhs->IsString()) {
		v8::String::Utf8Value val(rhs.As<v8::String>());

		m_type = VAR_STRING;
		m_data.str = (char*)malloc(val.length() + 1);
		strcpy(m_data.str, *val);
		return *this;
	}

	if (rhs->IsFunction()) {
		v8::Local<v8::Function> func = rhs.As<v8::Function>();
		v8::String::Utf8Value name(func->GetName());

		m_type = VAR_FUNCTION;
		m_data.str = (char*)malloc(name.length() + 1);
		strcpy(m_data.str, *name);
		return *this;
	}

	if (rhs->IsObject() && (rhs->IsFunction() == false)) {
		v8::Local<v8::Object> val = rhs->ToObject();
		void* valptr = val->GetAlignedPointerFromInternalField(0);
        GigaObject* obj = static_cast<GigaObject*>(valptr);
        bool assigned = false;
        
        if(obj->GetGigaName().compare("Vector3") == 0) {
            m_type = VAR_VECTOR3;
            
            Vector3* vec = (Vector3*)valptr;
            m_data.f1 = vec->x;
            m_data.f2 = vec->y;
            m_data.f3 = vec->z;
            
            assigned = true;
        }

        if(assigned == false) {
            m_type = VAR_OBJECT;
            m_data.obj = obj;
        }
        
		return *this;
	}

	return(*this);
}

v8::Local<v8::Value> ScriptVariant::GetValue() {
	v8::Isolate* isolate = v8::Isolate::GetCurrent();
	v8::EscapableHandleScope handle_scope(isolate);
	v8::Local<v8::Value> ret;

	// Get thread specific v8 variable
	ScriptThread* thread = (ScriptThread*)isolate->GetData(0);

	// Catch any errors the script might throw
	v8::TryCatch try_catch(isolate);

	if (IsInt()) {
		//printf("Returning int of value %d.\n", AsInt());
		ret = v8::Number::New(isolate, AsInt());
	}
	if (IsFloat()) {
		//printf("Returning float of value %.2f.\n", AsFloat());
		ret = v8::Number::New(isolate, AsFloat());
	}
	if (IsString()) {
		//printf("Returning string of value %s.\n", AsString().c_str());
		ret = v8::String::NewFromUtf8(isolate, AsString().c_str());
	}
	if (IsObject()) {
		GigaObject* obj = AsObject();
		//printf("Returning object of type %s.\n", obj->GetGigaName().c_str());
        ret = thread->GetJSObject(obj);
	}
    if (IsVector3()) {
        Vector3* vec = new Vector3(AsVector3());
        ret = thread->GetJSObject(vec);
    }
	if (IsFunction()) {
		v8::Local<v8::Object> globalSpace = isolate->GetCurrentContext()->Global();
		ret = globalSpace->Get(v8::String::NewFromUtf8(isolate, AsString().c_str()));
	}

	GIGA_ASSERT(ret.IsEmpty() == false, "Returned value cannot be read.");

	// Attempt to get an error message (need a better way to do this)
	v8::String::Utf8Value error(try_catch.Exception());
	if (*error) {
		// Bad
		assert(false);
	}

	return(handle_scope.Escape(ret));
}
