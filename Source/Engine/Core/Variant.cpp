
#include <Core/Variant.h>
#include <Core/DataRecordType.h>

Variant::Variant() {
	m_type = 0;
    m_size = 0;
}

Variant::~Variant() {
	if (m_type == VAR_STRING) {
		free(m_data.str);
	}
}

Variant::Variant(int32_t value) {
	*this = value;
}

Variant::Variant(uint32_t value) {
	*this = value;
}

Variant::Variant(int64_t value) {
	*this = value;
}

Variant::Variant(uint64_t value) {
	*this = value;
}

Variant::Variant(bool value) {
	*this = value;
}

Variant::Variant(float value) {
	*this = value;
}

Variant::Variant(vector2 value) {
	*this = value;
}

Variant::Variant(vector3 value) {
	*this = value;
}

Variant::Variant(vector4 value) {
	*this = value;
}

Variant::Variant(quaternion value) {
	*this = value;
}

Variant::Variant(std::string value) {
	*this = value;
}

Variant::Variant(GigaObject* value) {
	*this = value;
}

Variant::Variant(const Variant& value) {
	*this = value;
}

Variant& Variant::operator =(int32_t rhs) {
	m_type = VAR_INT32;
	m_data.i32 = rhs;
    m_size = sizeof(int32_t);
	return *this;
}

Variant& Variant::operator =(uint32_t rhs) {
	m_type = VAR_UINT32;
	m_data.ui32 = rhs;
    m_size = sizeof(uint32_t);
	return *this;
}

Variant& Variant::operator =(int64_t rhs) {
	m_type = VAR_INT64;
	m_data.i64 = rhs;
    m_size = sizeof(int64_t);
	return *this;
}

Variant& Variant::operator =(uint64_t rhs) {
	m_type = VAR_UINT64;
	m_data.ui64 = rhs;
    m_size = sizeof(uint64_t);
	return *this;
}

Variant& Variant::operator =(bool rhs) {
	m_type = VAR_BOOL;
	m_data.b = rhs;
    m_size = sizeof(bool);
	return *this;
}

Variant& Variant::operator =(float rhs) {
	m_type = VAR_FLOAT;
	m_data.f1 = rhs;
    m_size = sizeof(float);
	return *this;
}

Variant& Variant::operator =(vector2 rhs) {
	m_type = VAR_VECTOR2;
	m_data.f1 = rhs.x;
	m_data.f2 = rhs.y;
    m_size = sizeof(float) * 2;
	return *this;
}

Variant& Variant::operator =(vector3 rhs) {
	m_type = VAR_VECTOR3;
	m_data.f1 = rhs.x;
	m_data.f2 = rhs.y;
	m_data.f3 = rhs.z;
    m_size = sizeof(float) * 3;
	return *this;
}

Variant& Variant::operator =(vector4 rhs) {
	m_type = VAR_VECTOR4;
	m_data.f1 = rhs.x;
	m_data.f2 = rhs.y;
	m_data.f3 = rhs.z;
	m_data.f4 = rhs.w;
    m_size = sizeof(float) * 4;
	return *this;
}

Variant& Variant::operator =(quaternion rhs) {
	m_type = VAR_QUATERNION;
	m_data.f1 = rhs.x;
	m_data.f2 = rhs.y;
	m_data.f3 = rhs.z;
	m_data.f4 = rhs.w;
    m_size = sizeof(float) * 4;
	return *this;
}

Variant& Variant::operator =(std::string rhs) {
	m_type = VAR_STRING;
	m_data.str = (char*)malloc(rhs.length() + 1);
	strcpy(m_data.str, rhs.data());
    m_size = rhs.length() + 1;
	return *this;
}

Variant& Variant::operator =(GigaObject* rhs) {
	m_type = VAR_OBJECT;
	m_data.obj = rhs;
    
    // what to do about m_size here?
	return *this;
}

Variant& Variant::operator =(const Variant& rhs) {
	m_type = rhs.m_type;
	m_data = rhs.m_data;
    m_size = rhs.m_size;
	return *this;
}

bool Variant::IsInt() {
	return(m_type == VAR_INT32 || m_type == VAR_INT64);
}

bool Variant::IsUInt() {
	return(m_type == VAR_UINT32 || m_type == VAR_UINT64);
}

bool Variant::IsInt64() {
	return(m_type == VAR_INT64);
}

bool Variant::IsUInt64() {
	return(m_type == VAR_UINT64);
}

bool Variant::IsBool() {
	return(m_type == VAR_BOOL);
}

bool Variant::IsFloat() {
	return(m_type == VAR_FLOAT || m_type == VAR_INT32 || m_type == VAR_UINT32);
}

bool Variant::IsNumeric() {
	return(m_type == VAR_INT32 || VAR_UINT32 || VAR_INT64 || VAR_UINT64 || m_type == VAR_FLOAT);
}

bool Variant::IsVector2() {
	return(m_type == VAR_VECTOR2);
}

bool Variant::IsVector3() {
	return(m_type == VAR_VECTOR3);
}

bool Variant::IsVector4() {
	return(m_type == VAR_VECTOR4);
}

bool Variant::IsQuaternion() {
	return(m_type == VAR_QUATERNION);
}

bool Variant::IsString() {
	return(m_type == VAR_STRING);
}

bool Variant::IsObject() {
	return(m_type == VAR_OBJECT);
}

bool Variant::IsNull() {
	return(m_type == 0);
}

std::string Variant::ToString() {
	std::string output;
    int length = 0;
	switch (m_type) {
	case VAR_INT32:
		output = std::to_string(m_data.i32);
		break;
	case VAR_UINT32:
		output = std::to_string(m_data.ui32);
		break;
	case VAR_UINT64:
		output = std::to_string(m_data.ui64);
		break;
	case VAR_INT64:
		output = std::to_string(m_data.i64);
		break;
	case VAR_BOOL:
		output = m_data.b ? "true" : "false";
		break;
	case VAR_FLOAT:
		output = std::to_string(m_data.f1);
		break;
	case VAR_VECTOR2:
		output.resize(50);
		length = sprintf((char*)output.data(), "(%.2f, %.2f)", m_data.f1, m_data.f2);
        output.resize(length);
		break;
	case VAR_VECTOR3:
		output.resize(50);
		length = sprintf((char*)output.data(), "(%.2f, %.2f, %.2f)", m_data.f1, m_data.f2, m_data.f3);
        output.resize(length);
        break;
	case VAR_VECTOR4:
		output.resize(50);
		length = sprintf((char*)output.data(), "(%.2f, %.2f, %.2f, %.2f)", m_data.f1, m_data.f2, m_data.f3, m_data.f4);
        output.resize(length);
        break;
	case VAR_QUATERNION:
		output.resize(50);
		length = sprintf((char*)output.data(), "(%.2f, %.2f, %.2f, %.2f)", m_data.f4, m_data.f2, m_data.f3, m_data.f1);
        output.resize(length);
        break;
	case VAR_STRING:
		output = m_data.str;
		break;
    case VAR_OBJECT:
            // TODO: Add this back in
        break;
	default:
		break;
	}

	return(output);
}

void Variant::FromString(std::string value, int type) {
    switch (type) {
        case VAR_INT32:
            m_data.i32 = std::stoi(value);
            m_type = VAR_INT32;
            break;
        case VAR_UINT32:
            m_data.ui32 = (uint32_t)std::stoul(value);
            m_type = VAR_UINT32;
            break;
        case VAR_UINT64:
            m_data.i64 = (uint64_t)std::stoull(value);
            m_type = VAR_UINT64;
            break;
        case VAR_INT64:
            m_data.i64 = (uint64_t)std::stoll(value);
            m_type = VAR_INT64;
            break;
        case VAR_BOOL:
            m_data.b = value.compare("true") == 0 ? true : false;
            m_type = VAR_BOOL;
            break;
        case VAR_FLOAT:
            m_data.f1 = std::stof(value);
            m_type = VAR_FLOAT;
            break;
        case VAR_VECTOR2:
            sscanf(value.c_str(), "(%f, %f)", &m_data.f1, &m_data.f2);
            m_type = VAR_VECTOR2;
            break;
        case VAR_VECTOR3:
            sscanf(value.c_str(), "(%f, %f, %f)", &m_data.f1, &m_data.f2, &m_data.f3);
            m_type = VAR_VECTOR3;
            break;
        case VAR_VECTOR4:
            sscanf(value.c_str(), "(%f, %f, %f, %f)", &m_data.f1, &m_data.f2, &m_data.f3, &m_data.f4);
            m_type = VAR_VECTOR4;
            break;
        case VAR_QUATERNION:
            sscanf(value.c_str(), "(%f, %f, %f, %f)", &m_data.f4, &m_data.f2, &m_data.f3, &m_data.f1);
            m_type = VAR_QUATERNION;
            break;
        case VAR_STRING:
            m_data.str = (char*)malloc(value.size() + 1);
            strcpy(m_data.str, value.data());
            m_type = VAR_STRING;
            break;
        case VAR_OBJECT:
            // TODO: Add this back in
            break;
        default:
            break;
    }
}

int32_t Variant::AsInt() {
	if (m_type == VAR_INT32) {
		return m_data.i32;
	}

	if (m_type == VAR_FLOAT) {
		return((int)m_data.f1);
	}

	return(0);
}

uint32_t Variant::AsUInt() {
	if (m_type == VAR_UINT32) {
		return m_data.ui32;
	}

	if (m_type == VAR_FLOAT) {
		return((uint32_t)m_data.f1);
	}

	return(0);
}

int64_t Variant::AsInt64() {
	if (m_type == VAR_INT64) {
		return m_data.i64;
	}

	if (m_type == VAR_UINT64) {
		return((int64_t)m_data.ui64);
	}

	if (m_type == VAR_INT32) {
		return((int64_t)m_data.i32);
	}

	if (m_type == VAR_UINT32) {
		return((int64_t)m_data.ui32);
	}

	if (m_type == VAR_FLOAT) {
		return((int64_t)m_data.f1);
	}

	return(0);
}

uint64_t Variant::AsUInt64() {
	if (m_type == VAR_INT64) {
		return m_data.i64;
	}

	if (m_type == VAR_UINT64) {
		return((uint64_t)m_data.ui64);
	}

	if (m_type == VAR_INT32) {
		return((uint64_t)m_data.i32);
	}

	if (m_type == VAR_UINT32) {
		return((uint64_t)m_data.ui32);
	}

	if (m_type == VAR_FLOAT) {
		return((uint64_t)m_data.f1);
	}

	return(0);
}

bool Variant::AsBool() {
	if (m_type == VAR_BOOL) {
		return(m_data.b);
	}

	return(false);
}

float Variant::AsFloat() {
	if (m_type == VAR_FLOAT) {
		return(m_data.f1);
	}

	if (m_type == VAR_INT32) {
		return((float)m_data.i32);
	}

	if (m_type == VAR_UINT32) {
		return((float)m_data.ui32);
	}

	return(0.0f);
}

vector2 Variant::AsVector2() {
	if (m_type == VAR_VECTOR2) {
		return vector2(m_data.f1, m_data.f2);
	}

	return(vector2(0, 0));
}

vector3 Variant::AsVector3() {
	if (m_type == VAR_VECTOR3) {
		return vector3(m_data.f1, m_data.f2, m_data.f3);
	}

	return(vector3(0, 0, 0));
}

vector4 Variant::AsVector4() {
	if (m_type == VAR_VECTOR4) {
		return vector4(m_data.f1, m_data.f2, m_data.f3, m_data.f4);
	}

	return(vector4(0, 0, 0, 0));
}

quaternion Variant::AsQuaternion() {
	if (m_type == VAR_QUATERNION) {
		return quaternion(m_data.f4, m_data.f1, m_data.f2, m_data.f3);
	}

	return(quaternion(1, 0, 0, 0));
}

std::string Variant::AsString() {
	if (m_type == VAR_STRING) {
		return(m_data.str);
	}

	return(std::string(""));
}

GigaObject* Variant::AsObject() {
	if (m_type == VAR_OBJECT) {
		return m_data.obj;
	}

	return(0);
}

