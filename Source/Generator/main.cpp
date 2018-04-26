
#include <clang-c/Index.h>

#include <iostream>
#include <vector>
#include <map>

#include "MetaClass.h"
#include "Directory.h"

#ifdef WIN32
#include <direct.h>
#define mymkdir(path) _mkdir(path)
#else
#include <sys/stat.h>
#define mymkdir(path) mkdir(path, ACCESSPERMS)
#endif

using namespace std;

std::vector<const char*> args;
CXIndex cindex = 0;
CXTranslationUnit unit;
std::map<std::string, bool> GigaClasses;
std::map<std::string, std::string> inheritance;
std::string currentGigaClassName;
std::string currentClassName;
std::map<std::string, MetaClass*> classes;
std::map<std::string, bool> exportGigaClasses;
std::map<std::string, bool> singletonClasses;
bool exportGigaClass = false;
MetaClass* currentMetaClass = 0;
std::map<int, int> typeMappings;
std::map <int, std::string> functionMappings;
std::string precompiled_header;
std::vector<std::string> addlIncludes;
bool addedData = false;
bool pullOptions = false;
bool markSingleton = false;
bool markGet = false;
bool markSet = false;
bool markSerialize = false;

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

	if (pullOptions && cursor == CXCursor_EnumConstantDecl) {
		if (strcmp("Singleton", name.c_str()) == 0) {
			markSingleton = true;
		}

		if (strcmp("Get", name.c_str()) == 0) {
			markGet = true;
		}

		if (strcmp("Set", name.c_str()) == 0) {
			markSet = true;
		}

		if (strcmp("Serialize", name.c_str()) == 0) {
			markSerialize = true;
		}

		pullOptions = false;
		return CXChildVisit_Continue;
	}

	if (pullOptions && (cursor == CXCursor_CompoundStmt || cursor == CXCursor_DeclStmt || cursor == CXCursor_EnumDecl)) {
		return CXChildVisit_Recurse;
	}
    
    if (cursor == CXCursor_ClassDecl) {
        currentClassName = name;
        grabNextClass = true;
    }

    if (cursor == CXCursor_CXXBaseSpecifier) {
        inheritance[currentClassName] = name;
	}

	if (((grabNextFunction && cursor == CXCursor_CXXMethod) || (cursor == CXCursor_Constructor)) && currentMetaClass) {
		//cout << "Found GIGA function named '" << name.c_str() << "'" << endl;
		if (cursor == CXCursor_Constructor) {
			int num_args = clang_Cursor_getNumArguments(c);
			if (num_args == 0) {
				return CXChildVisit_Continue;
			}
		}
        
        CXType returnType = clang_getResultType(type);
		bool isStatic = clang_CXXMethod_isStatic(c);
        std::string rettype = clang_getCString(clang_getTypeSpelling(returnType));
        
        int internalType = map_internal_type(returnType, rettype);
        
        if(internalType != 0) {
            MetaClass::MetaFunction* func = new MetaClass::MetaFunction();
            func->name = name;
            func->returnType = internalType;
			func->objectType = name;
			func->isStatic = isStatic;
			func->isConstructor = (cursor == CXCursor_Constructor);
            
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
            
                //cout << "Arg " << i << " type '" << argtype.c_str() << "'" << endl;
            }

            currentMetaClass->functions[name] = func;
        }
        
        grabNextFunction = false;
		return CXChildVisit_Continue;
	}
    
    if (cursor == CXCursor_CXXMethod && name.compare("GFUNCTION") == 0) {
        grabNextFunction = true;
		return CXChildVisit_Continue;
    }
    
    if(cursor == CXCursor_FunctionDecl && currentGigaClassName.empty() == false) {
        int error = 1;
    }
    
    if (cursor == CXCursor_FunctionDecl && name.compare("GCLASS") == 0) {
        grabNextClass = true;
        exportGigaClass = true;
		pullOptions = true;
		markSingleton = false;
		return CXChildVisit_Recurse;
    }

	if (grabNextClass && cursor == CXCursor_ClassDecl) {
		//cout << "Found GIGA class named '" << name.c_str() << "'" << endl;
		currentGigaClassName = name;
        grabNextClass = false;
        
        if(exportGigaClass == true) {
            exportGigaClasses[name] = true;
        }
		
		if (markSingleton) {
			singletonClasses[name] = true;
			markSingleton = false;
		}

        std::map<std::string, MetaClass*>::iterator ci = classes.find(name);
		if (ci == classes.end() || exportGigaClass) {
			addedData = true;
            
            if(ci != classes.end()) {
                currentMetaClass = ci->second;
            }
            else {
                MetaClass* m = new MetaClass;
                m->name = name;
				m->singleton = false;

                classes[name] = m;
				currentMetaClass = m;
            }
            
            exportGigaClass = false;
		}
		else {
			currentMetaClass = ci->second;
		}

        exportGigaClass = false;
		return CXChildVisit_Recurse;
	}

	if (grabNextVar && cursor == CXCursor_FieldDecl && currentMetaClass) {
		//cout << "Found GIGA variable named '" << name.c_str() << "'" << endl;

        std::string typestr = clang_getCString(clang_getTypeSpelling(type));
        int internalType = map_internal_type(type, typestr);
        
        if(internalType != 0) {
            MetaClass::MetaVariable* var = new MetaClass::MetaVariable();
            var->name = name;
            var->type = internalType;
			var->get = var->set = var->serialize = false;

			if (markGet) {
				var->get = true;
				markGet = false;
			}

			if (markSet) {
				var->set = true;
				markSet = false;
			}

			if (markSerialize) {
				var->serialize = true;
				markSerialize = false;
			}
            
            currentMetaClass->variables[name] = var;
        }
        else {
            int error = 1;
        }
        
        grabNextVar = false;
        //cout << "Type '" << typestr.c_str() << "'" << endl;

		return CXChildVisit_Continue;
	}
    
    if (cursor == CXCursor_CXXMethod && name.compare("GVARIABLE") == 0) {
        grabNextVar = true;
		return CXChildVisit_Recurse;
    }

	clang_disposeString(str);

	if (cursor == CXCursor_ClassDecl && exportGigaClass == false) {
        addedData = false;
    }
    
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

		cout << filename << endl;

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

void AddInheritedClasses(MetaClass* cl, std::string name) {
    // Check if this class inherits from another
    std::map<std::string, std::string>::iterator it = inheritance.find(name);
    if(it != inheritance.end()) {
        // Get that class
        MetaClass* cl2 = classes[it->second];
        
        // If it does, add any meta functions from that class to this class too
        std::map<std::string, MetaClass::MetaFunction*>::iterator fi = cl2->functions.begin();
        for(; fi != cl2->functions.end(); fi++) {
			if (fi->second->isConstructor == true) continue;

            cl->functions[fi->second->name] = fi->second;
        }
        
        // Same for variables
        std::map<std::string, MetaClass::MetaVariable*>::iterator vi = cl2->variables.begin();
        for(; vi != cl2->variables.end(); vi++) {
            cl->variables[vi->second->name] = vi->second;
        }
        
        // Also check if that class inherits from another
        AddInheritedClasses(cl, it->second);
    }
}

int main(int argc, char** argv) {
	cindex = clang_createIndex(0, 0);

#ifndef WIN32
    args.push_back("-I/usr/local/include");
    args.push_back("-I/usr/include");
    args.push_back("-I/usr/include/c++/4.2.1");
#endif
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
    
    if(argc > 1) {
        current = argv[1];
    }

	std::string trailing = current.substr(current.length() - 1, 1);
	if (trailing == std::string("\\")) {
		current = current.substr(0, current.length() - 1);
	}
    
    // Trim 3 directories
    std::string path = current.substr(0, current.find_last_of("/\\"));
    path = path.substr(0, path.find_last_of("/\\"));
    path = path.substr(0, path.find_last_of("/\\"));

	std::string definePath = path + "/Source/Generator/Defines.h";
	std::string enginePath = "-I" + path + "/Source/Engine";

	args.push_back("-include");
	args.push_back(definePath.c_str());
	args.push_back(enginePath.c_str());

	cout << path << std::endl;
    
    // Bin directory
    std::string binDirectory = path + "/Bin";
    mymkdir(binDirectory.c_str());

	// Check for an existing precompiled header
	std::string autogenerated = binDirectory + "/generator.pch";
	FILE* fp = fopen(autogenerated.c_str(), "r");

	if (fp != 0) {
        cout << "Using precompiled header in" << binDirectory.c_str() << endl;
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
        
        std::map<std::string, bool>::iterator ei = exportGigaClasses.find(cl->name);
        if(ei == exportGigaClasses.end()) {
            continue;
        }

		std::map<std::string, bool>::iterator singleton = singletonClasses.find(cl->name);
		if (singleton != singletonClasses.end()) {
			cl->singleton = true;
		}
        
        // Process inheritance
        AddInheritedClasses(cl, cl->name);

		// Variables
		std::map<std::string, MetaClass::MetaVariable*>::iterator vi = cl->variables.begin();
		for (; vi != cl->variables.end(); vi++) {
			if (vi->second->get) {
				output += "Variant* meta_" + cl->name + "_" + vi->second->name + "_get(GigaObject* obj) {\n";
				output += "\t" + cl->name + "* cobj = dynamic_cast<" + cl->name + "*>(obj);\n";
				output += "\tGIGA_ASSERT(cobj != 0, \"Object is not of the correct type.\");\n\n";

				output += "\treturn(new Variant(cobj->" + vi->second->name + "));\n";
				output += "}\n\n";
			}
		}

		// Constructors
		bool hasConstructor = false;
		std::map<std::string, MetaClass::MetaFunction*>::iterator fi = cl->functions.begin();
		for (; fi != cl->functions.end(); fi++) {
			if (fi->second->isConstructor == false) continue;

			hasConstructor = true;
		}

		if (hasConstructor) {
			fi = cl->functions.begin();
			output += "Variant* meta_" + cl->name + "_New(GigaObject* obj, int argc, Variant** argv) {\n";

			for (; fi != cl->functions.end(); fi++) {
				if (fi->second->isConstructor == false) continue;

				output += "\tif(argc == " + std::to_string(fi->second->args.size()) + ") {\n";
				std::vector<MetaClass::MetaFunction::MetaArgument*>::iterator ai = fi->second->args.begin();
				int argc = 0;

				output += "\t\tif(true";
				for (; ai != fi->second->args.end(); ai++) {
					output += " && argv[" + std::to_string(argc) + "]->Is" + functionMappings[(*ai)->type] + "()";
					argc++;
				}
				output += ") {\n\t\t\treturn(new Variant(new " + cl->name + "(";

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

				output += ")));\n\t\t}\n\t}\n";
			}

			output += "\n\tGIGA_ASSERT(false, \"No matching constructor with those arguments.\");\n\treturn(0);";
			output += "\n}\n\n";
		}

		// Functions
		fi = cl->functions.begin();
		for (; fi != cl->functions.end(); fi++) {
			if (fi->second->isConstructor == true) continue;

			output += "Variant* meta_" + cl->name + "_" + fi->second->name + "(GigaObject* obj, int argc, Variant** argv) {\n";
			
			// Validate data
			std::vector<MetaClass::MetaFunction::MetaArgument*>::iterator ai = fi->second->args.begin();
			int argc = 0;
			for (; ai != fi->second->args.end(); ai++) {
				output += "\tGIGA_ASSERT(argv[" + std::to_string(argc) + "]->Is" + functionMappings[(*ai)->type] + "(), \"Incorrect type for argument " + std::to_string(argc) + ".\");\n";
				argc++;
			}

			output += "\n";

			if (fi->second->isStatic == false) {
				// Cast object
				if (cl->singleton == false) {
					output += "\t" + cl->name + "* cobj = dynamic_cast<" + cl->name + "*>(obj);\n";
					output += "\tGIGA_ASSERT(cobj != 0, \"Object is not of the correct type.\");\n";
				}
				else {
					output += "\tMetaSystem* metaSystem = GetSystem<MetaSystem>();\n";
					output += "\t" + cl->name + "* cobj = dynamic_cast<" + cl->name + "*>(metaSystem->GetSingleton(\"" + cl->name + "\"));\n";
					output += "\tGIGA_ASSERT(cobj != 0, \"Singleton class type not found.\");\n";
				}

				output += "\n";

				if (fi->second->returnType != -1) {
					output += "\treturn(new Variant(";
				}
				else {
					output += "\t";
				}
			}

			// Function call
			if (fi->second->isStatic == false) {
				output += "cobj->" + fi->first + "(";
			}
			else {
				output += "\t" + cl->name + "::" + fi->first + "(";
			}
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
	output += "void MetaSystem::RegisterMetaFunctions() {\n\tMetaSystem* metaSystem = GetSystem<MetaSystem>();\n\n";
	it = classes.begin();
	for (; it != classes.end(); it++) {
		MetaClass* cl = it->second;
        
        std::map<std::string, bool>::iterator ei = exportGigaClasses.find(cl->name);
        if(ei == exportGigaClasses.end()) {
            continue;
        }

		output += "\tmetaSystem->RegisterClassName<" + cl->name + ">(\"" + cl->name + "\");\n";

        std::map<std::string, MetaClass::MetaFunction*>::iterator fi = cl->functions.begin();
		for (; fi != cl->functions.end(); fi++) {
			std::string funcName = fi->second->isConstructor ? "New" : fi->first;
			output += "\tmetaSystem->RegisterFunction(\"" + cl->name + "\", \"" + funcName + "\", meta_" + cl->name + "_" + funcName + ", ";
			output += (fi->second->isStatic) ? "true" : "false";
			output += "); \n";
		}
        
        if(cl->functions.size()) {
            output += "\n";
        }
	}
	output += "}\n\n";

	std::string autogeneratedFolder = path + "/Source/Autogenerated";

	cout << "Writing output to " << autogeneratedFolder << endl;
	mymkdir(autogeneratedFolder.c_str());

	std::string outFile = autogeneratedFolder + "/MetaData.cpp";
	fp = fopen(outFile.c_str(), "w");
    if(fp != 0) {
        fwrite(output.data(), output.size(), 1, fp);
        fclose(fp);
    }
    else {
        cout << "Error writing to output file." << std::endl;
    }

	clang_disposeIndex(cindex);

	return(0);
}
