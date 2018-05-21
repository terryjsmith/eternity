
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
		bool isStatic;
		std::string objectType;
		bool isConstructor;

		struct MetaArgument {
			std::string name;
			int type;
			std::string objectType;
		};
        std::vector<MetaArgument*> args;
	};

	struct MetaVariable {
		std::string name;
		int type;
		bool get, set, serialize;
	};

public:
	std::string name;
    std::map<std::string, MetaFunction*> functions;
	std::map<std::string, MetaVariable*> variables;
	bool singleton;
};

#endif
