
#include <clang-c/Index.h>

#include <iostream>
#include <vector>
#include <map>

#include "MetaClass.h"

using namespace std;

bool haveClass = false;

CXTranslationUnit unit;
std::map<std::string, bool> GigaClasses;
std::string currentClassName;
std::string currentMetaName;
std::map<std::string, MetaClass*> classes;
MetaClass* currentMetaClass = 0;
std::map<int, int> typeMappings;

// Supported types
enum Type {
    VAR_INT = 1,
    VAR_UINT,
    VAR_BOOL,
    VAR_FLOAT,
    VAR_VECTOR2,
    VAR_VECTOR3,
    VAR_VECTOR4,
    VAR_MATRIX4,
    VAR_QUATERNION,
    VAR_STRING,
    VAR_OBJECT,
    VAR_ARRAY,
    VAR_LASTTYPE
};

ostream& operator<<(ostream& stream, const CXString& str)
{
	stream << clang_getCString(str);
	clang_disposeString(str);
	return stream;
}

int map_internal_type(CXType kind, std::string rettype) {
    int internalType = 0;
    if(typeMappings.find(kind.kind) != typeMappings.end()) {
        internalType = typeMappings[kind.kind];
    }
    
    if(internalType == 0 && kind.kind == CXTypeKind::CXType_Typedef) {
        if(rettype == "vector2") {
            internalType = VAR_VECTOR2;
        }
        
        if(rettype == "vector3") {
            internalType = VAR_VECTOR3;
        }
        
        if(rettype == "vector4") {
            internalType = VAR_VECTOR4;
        }
        
        if(rettype == "quaternion") {
            internalType = VAR_QUATERNION;
        }
        
        if(rettype == "matrix4") {
            internalType = VAR_MATRIX4;
        }
    }
    
    return(internalType);
}

CXChildVisitResult visitor(CXCursor c, CXCursor parent, CXClientData client_data) {
	CXCursorKind cursor = clang_getCursorKind(c);
	CXString str = clang_getCursorSpelling(c);
    CXType type = clang_getCursorType(c);
	std::string name = clang_getCString(str);

	if (cursor == CXCursor_ClassDecl && haveClass == false) {

	}

	if (cursor == CXCursor_ClassDecl) {
		currentClassName = name;
	}

	if (name.empty() == false) {
		size_t pos = name.find("class ");
		if (pos != name.npos) {
			name = name.substr(pos + 6);
		}
	}

	if ((cursor == CXCursor_CXXBaseSpecifier && name.find("GigaObject") != name.npos) || (GigaClasses.find(name) != GigaClasses.end())) {
		//cout << "Found GIGA base class named '" << currentClassName.c_str() << "'" << endl;
		GigaClasses[currentClassName] = true;
	}

	if (currentMetaName.empty() == false && cursor == CXCursor_CXXMethod) {
		cout << "Found GIGA function named '" << name.c_str() << "'" << endl;
        
        CXType returnType = clang_getResultType(type);
        std::string rettype = clang_getCString(clang_getTypeSpelling(returnType));
        cout << "Return type '" << rettype.c_str() << "'" << endl;
        
        int internalType = map_internal_type(returnType, rettype);
        
        if(internalType != 0) {
            MetaClass::MetaFunction* func = new MetaClass::MetaFunction();
            func->name = name;
            func->returnType = internalType;
            
            int num_args = clang_Cursor_getNumArguments(c);
            for(int i = 0; i < num_args; i++) {
                CXCursor arg = clang_Cursor_getArgument(c, i);
                CXString name = clang_getCursorSpelling(arg);
                std::string arg_name = clang_getCString(name);
            
                CXType arg_type = clang_getArgType(type, i);
                std::string argtype = clang_getCString(clang_getTypeSpelling(arg_type));
                
                int intArgType = map_internal_type(arg_type, argtype);
                if(intArgType != 0) {
                    func->args[argtype] = intArgType;
                }
                else {
                    int error = 1;
                }
            
                cout << "Arg " << i << " type '" << argtype.c_str() << "'" << endl;
            }

            currentMetaClass->functions.push_back(func);
        }
	}

	if (haveClass && cursor == CXCursor_ClassDecl) {
		cout << "Found GIGA class named '" << name.c_str() << "'" << endl;
		haveClass = false;
		currentMetaName = name;

		if (classes.find(name) == classes.end()) {
			MetaClass* m = new MetaClass;
			m->name = name;

			classes[name] = m;
			currentMetaClass = m;
		}
	}

	if (currentMetaName.empty() == false && cursor == CXCursor_FieldDecl) {
		cout << "Found GIGA variable named '" << name.c_str() << "'" << endl;

        std::string typestr = clang_getCString(clang_getTypeSpelling(type));
        int internalType = map_internal_type(type, typestr);
        
        if(internalType != 0) {
            MetaClass::MetaVariable* var = new MetaClass::MetaVariable();
            var->name = name;
            var->type = internalType;
            
            currentMetaClass->variables.push_back(var);
        }
        else {
            int error = 1;
        }
        
        cout << "Type '" << typestr.c_str() << "'" << endl;
	}

	if (cursor == CXCursor_FunctionDecl && name == "GCLASS") {
		haveClass = true;
	}

	if (currentMetaName.empty() == false) {
		cout << "Cursor '" << clang_getCursorSpelling(c) << "' of kind '"
			<< clang_getCursorKindSpelling(clang_getCursorKind(c)) << "'\n";
	}

	clang_disposeString(str);
	return CXChildVisit_Recurse;
}

int main(int argc, char** argv) {
	CXIndex index = clang_createIndex(0, 0);

	std::vector<const char*> args;
	args.push_back("-include");
	args.push_back("../Source/Generator/Defines.h");
	args.push_back("-I../Source/Engine");
	args.push_back("-x");
	args.push_back("c++");
    
    // Create type mappings
    typeMappings[CXTypeKind::CXType_Void] = -1;
    typeMappings[CXTypeKind::CXType_Bool] = VAR_BOOL;
    typeMappings[CXTypeKind::CXType_Int] = VAR_INT;
    typeMappings[CXTypeKind::CXType_UInt] = VAR_UINT;
    typeMappings[CXTypeKind::CXType_Float] = VAR_FLOAT;
    typeMappings[CXTypeKind::CXType_Pointer] = VAR_OBJECT;
    typeMappings[CXTypeKind::CXType_SChar] = VAR_STRING;

	int flags = CXTranslationUnit_Incomplete | CXTranslationUnit_KeepGoing | CXTranslationUnit_DetailedPreprocessingRecord;

	clang_parseTranslationUnit2(
		index,
		"../Source/Engine/Render/MeshComponent.h", args.data(), args.size(),
		nullptr, 0,
		CXTranslationUnit_None, &unit);
	if (unit == nullptr) 	{
		cerr << "Unable to parse translation unit. Quitting." << endl;
		exit(-1);
	}

	CXCursor cursor = clang_getTranslationUnitCursor(unit);
	clang_visitChildren(cursor, visitor, nullptr);

	clang_disposeTranslationUnit(unit);
	clang_disposeIndex(index);

	return(0);
}
