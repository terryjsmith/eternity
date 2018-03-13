
#include <clang-c/Index.h>

#include <iostream>
#include <vector>
using namespace std;

bool haveClass = false;
bool haveFunction = false;
bool haveVariable = false;
CXTranslationUnit unit;

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

	if (haveFunction && cursor == CXCursor_CXXMethod) {
		cout << "Found GIGA function named '" << name.c_str() << "'" << endl;
		haveFunction = false;
	}

	if (haveClass && cursor == CXCursor_ClassDecl) {
		cout << "Found GIGA class named '" << name.c_str() << "'" << endl;
		haveClass = false;
	}

	if (haveVariable && cursor == CXCursor_FieldDecl) {
		cout << "Found GIGA variable named '" << name.c_str() << "'" << endl;
		haveVariable = false;
	}

	if (cursor == CXCursor_FunctionDecl && name == "GCLASS") {
		haveClass = true;
	}

	if (cursor == CXCursor_CXXMethod && name == "GFUNCTION") {
		haveFunction = true;
	}

	if (cursor == CXCursor_CXXMethod && name == "GVARIABLE") {
		haveVariable = true;
	}

	if (haveClass || haveFunction || haveVariable) {
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