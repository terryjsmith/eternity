
#ifndef material_h
#define material_h

#include <eternity.h>
#include <Core/MathTypes.h>

/**
 * Material color/reflection data
 */
class GIGA_API Material {
public:
    Material() : emissionStrength(0.0f), shininess(0.0f), m_materialID(0) { }
    ~Material() = default;
    
    unsigned int GetMaterial() { return m_materialID; }
    void SetMaterialID(unsigned int id) { m_materialID = id; }
    
public:
    vector4 diffuse;
    vector4 ambient;
    vector4 specular;
    vector3 emissionColor;
    float emissionStrength;
    float shininess;
    
protected:
    // Material ID assigned by material system
    unsigned int m_materialID;
};

#endif
