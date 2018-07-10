
#ifndef variant_hpp
#define variant_hpp

#include <eternity.h>
#include <Core/GigaObject.h>
#include <Core/MathTypes.h>

class GIGA_API Variant {
public:
	Variant();
	virtual ~Variant();

	// Supported types
	enum Type {
		VAR_INT32 = 1,
		VAR_UINT32,
		VAR_INT64,
		VAR_UINT64,
		VAR_BOOL,
		VAR_FLOAT, 
		VAR_VECTOR2,
		VAR_VECTOR3,
		VAR_VECTOR4,
		VAR_QUATERNION,
		VAR_STRING,
		VAR_OBJECT,
		VAR_ARRAY,
		VAR_LASTTYPE
	};

	struct Value {
		union {
			int32_t i32;
			uint32_t ui32;
			int64_t i64;
			uint64_t ui64;
			bool b;
			float f1;
			char* str;
			void* ptr;
			GigaObject* obj;
		};

		union {
			float f2;
		};

		union {
			float f3;
		};

		union {
			float f4;
		};
	};

	/**
	* Constructors
	*/
	Variant(int32_t value);
	Variant(uint32_t value);
	Variant(int64_t value);
	Variant(uint64_t value);
	Variant(bool value);
	Variant(float value);
	Variant(vector2 value);
	Variant(vector3 value);
	Variant(vector4 value);
	Variant(quaternion value);
	Variant(std::string value);
	Variant(GigaObject* value);
	Variant(const Variant& value);

	/**
	* Set operators
	*/
	Variant& operator =(int32_t rhs);
	Variant& operator =(uint32_t rhs);
	Variant& operator =(int64_t rhs);
	Variant& operator =(uint64_t rhs);
	Variant& operator =(bool rhs);
	Variant& operator =(float rhs);
	Variant& operator =(vector2 rhs);
	Variant& operator =(vector3 rhs);
	Variant& operator =(vector4 rhs);
	Variant& operator =(quaternion rhs);
	Variant& operator =(std::string rhs);
	Variant& operator =(GigaObject* rhs);
	Variant& operator =(const Variant& rhs);

	/**
	* Checkers
	*/
	bool IsInt();
	bool IsUInt();
	bool IsInt64();
	bool IsUInt64();
	bool IsBool();
	bool IsFloat();
	bool IsNumeric();
	bool IsVector2();
	bool IsVector3();
	bool IsVector4();
	bool IsQuaternion();
	bool IsString();
	bool IsObject();
	bool IsNull();

	/**
	* Convert value to string
	*/
	std::string ToString();
    
    /**
     * Convert value from string
     */
    void FromString(std::string value, int type);

	/**
	* Get operators
	*/
	int32_t AsInt();
	uint32_t AsUInt();
	int64_t AsInt64();
	uint64_t AsUInt64();
	bool AsBool();
	float AsFloat();
	vector2 AsVector2();
    vector3 AsVector3();
    vector4 AsVector4();
    quaternion AsQuaternion();
	std::string AsString();
	GigaObject* AsObject();
	template<class T> T AsObject() {
		if (m_type == VAR_OBJECT) {
			return(dynamic_cast<T>(m_data.obj));
		}

		return(0);
	}

protected:
	int m_type;
	Value m_data;
};

#endif
