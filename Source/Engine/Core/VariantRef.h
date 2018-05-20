
#ifndef variantref_hpp
#define variantref_hpp

#include <eternity.h>
#include <Core/GigaObject.h>
#include <Core/MathTypes.h>

template<class T>
class GIGA_API VariantRef {
public:
    VariantRef() : m_type(0) { }
    ~VariantRef() = default;
    
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
            int32_t T::*i32;
            uint32_t T::*ui32;
            int64_t T::*i64;
            uint64_t T::*ui64;
            bool T::*b;
            float T::*f1;
            std::string T::*str;
            void* T::*ptr;
            vector2 T::*vec2;
            vector3 T::*vec3;
            vector4 T::*vec4;
            quaternion T::*quat;
            GigaObject* T::*obj;
        };
    };
    
    /**
     * Constructors
     */
    VariantRef(int32_t T::*value) {
        *this = value;
    }
    
    VariantRef(uint32_t T::*value) {
        *this = value;
    }
    
    VariantRef(int64_t T::*value) {
        *this = value;
    }
    
    VariantRef(uint64_t T::* value) {
        *this = value;
    }
    
    VariantRef(bool T::* value) {
        *this = value;
    }
    
    VariantRef(float T::* value) {
        *this = value;
    }
    
    VariantRef(vector2 T::* value) {
        *this = value;
    }
    
    VariantRef(vector3 T::* value) {
        *this = value;
    }
    
    VariantRef(vector4 T::* value) {
        *this = value;
    }
    
    VariantRef(quaternion T::* value) {
        *this = value;
    }
    
    VariantRef(std::string T::* value) {
        *this = value;
    }
    
    VariantRef(GigaObject* T::* value) {
        *this = value;
    }
    
    VariantRef(const VariantRef& value) {
        *this = value;
    }
    
    /**
     * Set operators
     */
    VariantRef& operator =(int32_t T::* rhs) {
        m_type = VAR_INT32;
        m_data.i32 = rhs;
        return *this;
    }
    
    VariantRef& operator =(uint32_t T::* rhs) {
        m_type = VAR_UINT32;
        m_data.ui32 = rhs;
        return *this;
    }
    
    VariantRef& operator =(int64_t T::* rhs) {
        m_type = VAR_INT64;
        m_data.i64 = rhs;
        return *this;
    }
    
    VariantRef& operator =(uint64_t T::* rhs) {
        m_type = VAR_UINT64;
        m_data.ui64 = rhs;
        return *this;
    }
    
    VariantRef& operator =(bool T::* rhs) {
        m_type = VAR_BOOL;
        m_data.b = rhs;
        return *this;
    }
    
    VariantRef& operator =(float T::* rhs) {
        m_type = VAR_FLOAT;
        m_data.f1 = rhs;
        return *this;
    }
    
    VariantRef& operator =(vector2 T::* rhs) {
        m_type = VAR_VECTOR2;
        m_data.vec2 = rhs;
        return *this;
    }
    
    VariantRef& operator =(vector3 T::* rhs) {
        m_type = VAR_VECTOR3;
        m_data.vec3 = rhs;
        return *this;
    }
    
    VariantRef& operator =(vector4 T::* rhs) {
        m_type = VAR_VECTOR4;
        m_data.vec4 = rhs;
        return *this;
    }
    
    VariantRef& operator =(quaternion T::* rhs) {
        m_type = VAR_QUATERNION;
        m_data.quat = rhs;
        return *this;
    }
    
    VariantRef& operator =(std::string T::* rhs) {
        m_type = VAR_STRING;
        m_data.str = rhs;
        return *this;
    }
    
    VariantRef& operator =(GigaObject* T::* rhs) {
        m_type = VAR_OBJECT;
        m_data.obj = rhs;
        return *this;
    }
    
    VariantRef& operator =(const VariantRef& rhs) {
        m_type = rhs.m_type;
        m_data = rhs.m_data;
        return *this;
    }
    
    /**
     * Checkers
     */
    bool IsInt() {
        return(m_type == VAR_INT32 || m_type == VAR_INT64);
    }
    
    bool IsUInt() {
        return(m_type == VAR_UINT32 || m_type == VAR_UINT64);
    }
    
    bool IsInt64() {
        return(m_type == VAR_INT64);
    }
    
    bool IsUInt64() {
        return(m_type == VAR_UINT64);
    }
    
    bool IsBool() {
        return(m_type == VAR_BOOL);
    }
    
    bool IsFloat() {
        return(m_type == VAR_FLOAT || m_type == VAR_INT32 || m_type == VAR_UINT32);
    }
    
    bool IsNumeric() {
        return(m_type == VAR_INT32 || VAR_UINT32 || VAR_INT64 || VAR_UINT64 || m_type == VAR_FLOAT);
    }
    
    bool IsVector2() {
        return(m_type == VAR_VECTOR2);
    }
    
    bool IsVector3() {
        return(m_type == VAR_VECTOR3);
    }
    
    bool IsVector4() {
        return(m_type == VAR_VECTOR4);
    }
    
    bool IsQuaternion() {
        return(m_type == VAR_QUATERNION);
    }
    
    bool IsString() {
        return(m_type == VAR_STRING);
    }
    
    bool IsObject() {
        return(m_type == VAR_OBJECT);
    }
    
    bool IsNull() {
        return(m_type == 0);
    }
    
    /**
     * Get operators
     */
    int32_t T::* AsInt() {
        if (m_type == VAR_INT32) {
            return m_data.i32;
        }
        
        if (m_type == VAR_FLOAT) {
            return((int*)m_data.f1);
        }
        
        return(0);
    }
    
    uint32_t T::* AsUInt() {
        if (m_type == VAR_UINT32) {
            return m_data.ui32;
        }
        
        if (m_type == VAR_FLOAT) {
            return((uint32_t*)m_data.f1);
        }
        
        return(0);
    }
    
    int64_t T::* AsInt64() {
        if (m_type == VAR_INT64) {
            return m_data.i64;
        }
        
        if (m_type == VAR_UINT64) {
            return((int64_t*)m_data.ui64);
        }
        
        if (m_type == VAR_INT32) {
            return((int64_t*)m_data.i32);
        }
        
        if (m_type == VAR_UINT32) {
            return((int64_t*)m_data.ui32);
        }
        
        if (m_type == VAR_FLOAT) {
            return((int64_t*)m_data.f1);
        }
        
        return(0);
    }
    
    uint64_t T::* AsUInt64() {
        if (m_type == VAR_INT64) {
            return (uint64_t*)m_data.i64;
        }
        
        if (m_type == VAR_UINT64) {
            return((uint64_t*)m_data.ui64);
        }
        
        if (m_type == VAR_INT32) {
            return((uint64_t*)m_data.i32);
        }
        
        if (m_type == VAR_UINT32) {
            return((uint64_t*)m_data.ui32);
        }
        
        if (m_type == VAR_FLOAT) {
            return((uint64_t*)m_data.f1);
        }
        
        return(0);
    }
    
    bool T::* AsBool() {
        if (m_type == VAR_BOOL) {
            return(m_data.b);
        }
        
        return(0);
    }
    
    float T::* AsFloat() {
        if (m_type == VAR_FLOAT) {
            return(m_data.f1);
        }
        
        if (m_type == VAR_INT32) {
            return((float*)m_data.i32);
        }
        
        if (m_type == VAR_UINT32) {
            return((float*)m_data.ui32);
        }
        
        return(0);
    }
    
    vector2 T::* AsVector2() {
        if (m_type == VAR_VECTOR2) {
            return m_data.vec2;
        }
        
        return(0);
    }
    
    vector3 T::* AsVector3() {
        if (m_type == VAR_VECTOR3) {
            return m_data.vec3;
        }
        
        return(0);
    }
    
    vector4 T::* AsVector4() {
        if (m_type == VAR_VECTOR4) {
            return m_data.vec4;
        }
        
        return(0);
    }
    
    quaternion T::* AsQuaternion() {
        if (m_type == VAR_QUATERNION) {
            return m_data.quat;
        }
        
        return(0);
    }
    
    std::string T::* AsString() {
        if (m_type == VAR_STRING) {
            return(m_data.str);
        }
        
        return(0);
    }
    
    GigaObject* T::* AsObject() {
        if (m_type == VAR_OBJECT) {
            return m_data.obj;
        }
        
        return(0);
    }

    template<class C> C T::* AsObject() {
        if (m_type == VAR_OBJECT) {
            return(dynamic_cast<C T::*>(m_data.obj));
        }
        
        return(0);
    }
    
protected:
    int m_type;
    Value m_data;
};

#endif
