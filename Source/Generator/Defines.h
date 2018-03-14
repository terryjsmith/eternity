
#pragma once

#define GIGA_API 
#define GIGA_FUNCTION(...) void GFUNCTION() { enum { __VA_ARGS__ }; }
#define GIGA_CLASS(...) void GCLASS() { enum { __VA_ARGS__ }; }
#define GIGA_VARIABLE(...) GVARIABLE() { enum { __VA_ARGS__ }; }
