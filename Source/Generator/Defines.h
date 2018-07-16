
#pragma once

void FNAME(char* name);

#define GIGA_API 
#define GIGA_FUNCTION(...) void GFUNCTION() { enum { __VA_ARGS__ }; }
#define GIGA_CLASS(...) void GCLASS() { enum { __VA_ARGS__ }; }
#define GIGA_VARIABLE(...) void GVARIABLE() { enum { __VA_ARGS__ }; }
#define GIGA_CLASS_BODY(...) void GBODY() { enum { __VA_ARGS__ }; }
