
#include <clang-c/Index.h>

#include <iostream>
#include <vector>
#include <map>

#include "MetaClass.h"
#include "Directory.h"

#ifdef WIN32
#include <direct.h>
#define mymkdir _mkdir
#endif

using namespace std;

std::vector<const char*> args;
CXIndex cindex = 0;
CXTranslationUnit unit;
std::map<std::string, bool> GigaClasses;
std::string currentGigaClassName;
std::string currentClassName;
std::map<std::string, MetaClass*> classes;
MetaClass* currentMetaClass = 0;
std::map<int, int> typeMappings;
std::map <int, std::string> functionMappings;
std::string precompiled_header;
std::vector<std::string> addlIncludes;
bool addedData = false;

bool grabNextFunction = false;
bool grabNextVar = false;
bool grabNextClass = false;

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

	if (internalType == 0 && rettype == "std::string") {
		internalType = VAR_STRING;
	}
    
    return(internalType);
}

CXChildVisitResult visitor(CXCursor c, CXCursor parent, CXClientData client_data) {
	CXCursorKind cursor = clang_getCursorKind(c);
	CXString str = clang_getCursorSpelling(c);
    CXType type = clang_getCursorType(c);
	std::string name = clang_getCString(str);

	if (name.empty() == false) {
		size_t pos = name.find("class ");
		if (pos != name.npos) {
			name = name.substr(pos + 6);
		}
	}
    
    if (cursor == CXCursor_ClassDecl) {
        currentClassName = name;
    }

	if ((cursor == CXCursor_CXXBaseSpecifier && name.find("GigaObject") != name.npos) || (GigaClasses.find(name) != GigaClasses.end())) {
		//cout << "Found GIGA base class named '" << currentClassName.c_str() << "'" << endl;
		GigaClasses[currentClassName] = true;
	}

	if (grabNextFunction && cursor == CXCursor_CXXMethod && currentMetaClass) {
		cout << "Found GIGA function named '" << name.c_str() << "'" << endl;
        
        CXType returnType = clang_getResultType(type);
        std::string rettype = clang_getCString(clang_getTypeSpelling(returnType));
        
        int internalType = map_internal_type(returnType, rettype);
        
        if(internalType != 0) {
			addedData = true;

            MetaClass::MetaFunction* func = new MetaClass::MetaFunction();
            func->name = name;
            func->returnType = internalType;
			func->objectType = name;
            
            int num_args = clang_Cursor_getNumArguments(c);
            for(int i = 0; i < num_args; i++) {
                CXCursor arg = clang_Cursor_getArgument(c, i);
                CXString n = clang_getCursorSpelling(arg);
                std::string arg_name = clang_getCString(n);
            
                CXType arg_type = clang_getArgType(type, i);
                std::string argtype = clang_getCString(clang_getTypeSpelling(arg_type));
                
                int intArgType = map_internal_type(arg_type, argtype);
                if(intArgType != 0) {
					MetaClass::MetaFunction::MetaArgument* a = new MetaClass::MetaFunction::MetaArgument();
					a->name = arg_name;
					a->type = intArgType;
					a->objectType = argtype;
                    
					func->args.push_back(a);
                }
                else {
                    int error = 1;
                }
            
                cout << "Arg " << i << " type '" << argtype.c_str() << "'" << endl;
            }

            currentMetaClass->functions[name] = func;
        }
        
        grabNextFunction = false;
		return CXChildVisit_Continue;
	}
    
    if (cursor == CXCursor_CXXMethod && name.compare("GFUNCTION") == 0) {
        grabNextFunction = true;
		return CXChildVisit_Recurse;
    }
    
    if(cursor == CXCursor_FunctionDecl && currentGigaClassName.empty() == false) {
        int error = 1;
    }
    
    if (cursor == CXCursor_FunctionDecl && name.compare("GCLASS") == 0) {
        grabNextClass = true;
		return CXChildVisit_Recurse;
    }

	if (grabNextClass && cursor == CXCursor_ClassDecl) {
		cout << "Found GIGA class named '" << name.c_str() << "'" << endl;
		currentGigaClassName = name;

		if (classes.find(name) == classes.end()) {
			addedData = true;

			MetaClass* m = new MetaClass;
			m->name = name;

			classes[name] = m;
			currentMetaClass = m;
		}
        
        grabNextClass = false;

		return CXChildVisit_Recurse;
	}

	if (grabNextVar && cursor == CXCursor_FieldDecl && currentMetaClass) {
		cout << "Found GIGA variable named '" << name.c_str() << "'" << endl;

        std::string typestr = clang_getCString(clang_getTypeSpelling(type));
        int internalType = map_internal_type(type, typestr);
        
        if(internalType != 0) {
			addedData = true;

            MetaClass::MetaVariable* var = new MetaClass::MetaVariable();
            var->name = name;
            var->type = internalType;
            
            currentMetaClass->variables[name] = var;
        }
        else {
            int error = 1;
        }
        
        grabNextVar = false;
        cout << "Type '" << typestr.c_str() << "'" << endl;

		return CXChildVisit_Continue;
	}
    
    if (cursor == CXCursor_CXXMethod && name.compare("GVARIABLE") == 0) {
        grabNextVar = true;
		return CXChildVisit_Recurse;
    }

	clang_disposeString(str);

	if (cursor == CXCursor_ClassDecl) {
		return CXChildVisit_Recurse;
	}

	return CXChildVisit_Continue;
}

void ProcessDirectory(Directory* dir) {
	std::vector<std::string>::iterator fi = dir->files.begin();
	for (; fi != dir->files.end(); fi++) {
		addedData = false;

		std::string filename = dir->path + "/" + (*fi);
		size_t pos = filename.find_last_of(".");
		std::string extension = filename.substr(pos, filename.length() - pos);

		if (extension.compare(".h") != 0) {
			continue;
		}

		int flags = (precompiled_header.empty() ?  CXTranslationUnit_ForSerialization : 0);

		clang_parseTranslationUnit2(
			cindex,
			filename.c_str(), args.data(), (int)args.size(),
			nullptr, 0,
			flags, &unit);
		if (unit == nullptr) {
			cerr << "Unable to parse translation unit. Quitting." << endl;
			exit(-1);
		}

		CXCursor cursor = clang_getTranslationUnitCursor(unit);
		clang_visitChildren(cursor, visitor, nullptr);

		if (precompiled_header.empty()) {
			clang_saveTranslationUnit(unit, "generator.pch", clang_defaultSaveOptions(unit));
			precompiled_header = "generator.pch";

			args.push_back("-include-pch");
			args.push_back(precompiled_header.c_str());
		}

		if (addedData) {
			addlIncludes.push_back(filename);
		}

		clang_disposeTranslationUnit(unit);
        currentGigaClassName.clear();
        currentMetaClass = 0;
	}
    
    std::vector<Directory*>::iterator di = dir->subdirectories.begin();
    for (; di != dir->subdirectories.end(); di++) {
        ProcessDirectory(*di);
    }
}

int main(int argc, char** argv) {
	cindex = clang_createIndex(0, 0);

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

	// Mappings to functions
	functionMappings[VAR_INT] = "Int";
	functionMappings[VAR_UINT] = "UInt";
	functionMappings[VAR_BOOL] = "Bool";
	functionMappings[VAR_FLOAT] = "Float";
	functionMappings[VAR_VECTOR2] = "Vector2";
	functionMappings[VAR_VECTOR3] = "Vector3";
	functionMappings[VAR_VECTOR4] = "Vector4";
	functionMappings[VAR_MATRIX4] = "Matrix4";
	functionMappings[VAR_QUATERNION] = "Quaternion";
	functionMappings[VAR_STRING] = "String";
	functionMappings[VAR_OBJECT] = "Object";		

	std::string current = Directory::GetCurrent();
	std::string path = current.substr(0, current.find_last_of("/\\"));

	// Check for an existing precompiled header
	std::string autogenerated = current + "/generator.pch";
	FILE* fp = fopen(autogenerated.c_str(), "r");

	if (fp != 0) {
		precompiled_header = "generator.pch";

		args.push_back("-include-pch");
		args.push_back(precompiled_header.c_str());
		fclose(fp);
	}

	Directory* dir = new Directory();
	dir->Open(path +"/Source/Engine");

	ProcessDirectory(dir);

	std::vector<std::string> classNames;
	std::string output = "#include <Core/MetaSystem.h>\n#include <Core/Application.h>\n";
	std::string sourceDir = path + "/Source/Engine/";
	for (size_t i = 0; i < addlIncludes.size(); i++) {
		std::string include = addlIncludes[i].substr(sourceDir.length());
		output += "#include <" + include + ">\n";
	}
	output += "\n";

	// Write new functions for each meta class
	std::map<std::string, MetaClass*>::iterator it = classes.begin();
	for (; it != classes.end(); it++) {
		MetaClass* cl = it->second;
		
		// Functions
		std::map<std::string, MetaClass::MetaFunction*>::iterator fi = cl->functions.begin();
		for (; fi != cl->functions.end(); fi++) {
			output += "Variant* meta_" + cl->name + "_" + fi->second->name + "(GigaObject* obj, int argc, Variant** argv) {\n";
			
			// Validate data
			std::vector<MetaClass::MetaFunction::MetaArgument*>::iterator ai = fi->second->args.begin();
			int argc = 0;
			for (; ai != fi->second->args.end(); ai++) {
				output += "\tGIGA_ASSERT(argv[" + std::to_string(argc) + "]->Is" + functionMappings[(*ai)->type] + "(), \"Incorrect type for argument " + std::to_string(argc) + ".\");\n\n";
				argc++;
			}

			// Cast object
			output += "\t" + cl->name + "* cobj = dynamic_cast<" + cl->name + "*>(obj);\n";
			output += "\tGIGA_ASSERT(cobj != 0, \"Object is not of the correct type.\");\n\n";

			if (fi->second->returnType != -1) {
				output += "\treturn(new Variant(";
			}
			else {
				output += "\t";
			}

			// Function call
			output += "cobj->" + fi->first + "(";
			ai = fi->second->args.begin();
			argc = 0;
			for (; ai != fi->second->args.end(); ai++) {
				output += "argv[" + std::to_string(argc) + "]->As" + functionMappings[(*ai)->type];
				if ((*ai)->type == VAR_OBJECT) {
					output += "<" + (*ai)->objectType + ">";
				}
				output += "(),";
				argc++;
			}
			// Trim final comma
			if (argc > 0) {
				output = output.substr(0, output.length() - 1);
			}
			else {
				//output += ")";
			}

			output += ")";
			if (fi->second->returnType == -1) {
				output += ";\n\treturn(new Variant(0));";
			}
			else {
				output += "));";
			}

			output += "\n}\n\n";
		}
	}

	// Registration
	output += "void RegisterMetaFunctions() {\n\tMetaSystem* metaSystem = GetSystem<MetaSystem>();\n\n";
	it = classes.begin();
	for (; it != classes.end(); it++) {
		MetaClass* cl = it->second;

        std::map<std::string, MetaClass::MetaFunction*>::iterator fi = cl->functions.begin();
		for (; fi != cl->functions.end(); fi++) {
			output += "\tmetaSystem->RegisterFunction(\"" + cl->name + "\", \"" + fi->first + "\", meta_" + cl->name + "_" + fi->first + ");\n";
		}
		output += "\n";
	}
	output += "}\n\n";

	std::string autogeneratedFolder = path + "/Source/Autogenerated";
	mymkdir(autogeneratedFolder.c_str());

	std::string outFile = autogeneratedFolder + "/MetaData.cpp";
	fp = fopen(outFile.c_str(), "w");
	fwrite(output.data(), output.size(), 1, fp);
	fclose(fp);

	clang_disposeIndex(cindex);

	return(0);
}
