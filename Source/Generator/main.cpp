
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

ostream& operator<<(ostream& stream, const CXString& str)
{
	stream << clang_getCString(str);
	clang_disposeString(str);
	return stream;
}

void printToken(CXCursor c) {
	CXSourceRange srcRange = clang_getCursorExtent(c);
	CXToken *tokens;
	unsigned int nbTokens;
	CXTranslationUnit transUnit = clang_Cursor_getTranslationUnit(c);

	clang_tokenize(unit, srcRange, &tokens, &nbTokens);

	for (int i = 0; i < nbTokens; ++i)
	{
		CXToken currentToken = tokens[i];
		CXString tokenStr = clang_getTokenSpelling(transUnit, currentToken);
		printf("Token: %s\n", clang_getCString(tokenStr));
		clang_disposeString(tokenStr);
	}

	clang_disposeTokens(transUnit, tokens, nbTokens);
}

CXChildVisitResult visitor(CXCursor c, CXCursor parent, CXClientData client_data) {
	CXCursorKind cursor = clang_getCursorKind(c);
	CXString str = clang_getCursorSpelling(c);
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
		MetaClass::MetaFunction* func = new MetaClass::MetaFunction();
		func->name = name;

		currentMetaClass->functions.push_back(func);
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

		MetaClass::MetaVariable* var = new MetaClass::MetaVariable();
		var->name = name;

		currentMetaClass->variables.push_back(var);
	}

	if (cursor == CXCursor_FunctionDecl && name == "GCLASS") {
		haveClass = true;
	}

	/*if (currentMetaName.empty() == false) {
		cout << "Cursor '" << clang_getCursorSpelling(c) << "' of kind '"
			<< clang_getCursorKindSpelling(clang_getCursorKind(c)) << "'\n";
	}*/

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
