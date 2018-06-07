
#ifndef shaderdefinitionset_h
#define shaderdefinitionset_h

#include <eternity.h>

/**
 * A list of shader variations/definitions
 */
class GIGA_API ShaderDefinitionSet {
public:
    ShaderDefinitionSet() = default;
    ~ShaderDefinitionSet() = default;
    
    void Add(std::string define) { definitions[define] = true; }
    
    ShaderDefinitionSet(const ShaderDefinitionSet &rhs) {
        this->definitions = rhs.definitions;
    }
    
    ShaderDefinitionSet& operator = (const ShaderDefinitionSet &rhs) {
        this->definitions = rhs.definitions;
        return(*this);
    }
    
public:
    std::map<std::string, bool> definitions;
};


#endif
