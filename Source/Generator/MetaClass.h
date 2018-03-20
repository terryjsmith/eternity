
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
		std::string objectType;

		struct MetaArgument {
			std::string name;
			int type;
			std::string objectType;
		};
        std::vector<MetaArgument*> args;
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
