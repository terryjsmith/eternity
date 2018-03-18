
#ifndef metaclass_h
#define metaclass_h

#include <vector>
#include <string>
#include <map>

class MetaClass {
public:
	MetaClass() = default;
	~MetaClass() = default;

	struct MetaFunction {
		std::string name;
        int returnType;
        std::map<std::string, int> args;
	};

	struct MetaVariable {
		std::string name;
		std::string type;
	};

public:
	std::string name;
	std::vector<MetaFunction*> functions;
	std::vector<MetaVariable*> variables;
};

#endif
