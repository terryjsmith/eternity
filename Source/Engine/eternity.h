
#pragma once

// Disable compiler warnings about using STL and dynamic linking
#pragma warning(disable:4251)

// Windows includes
#ifdef WIN32
	#define NOMINMAX
	#define WIN32_LEAN_AND_MEAN
	#include <Windows.h>
#else
    #include <unistd.h>
#endif

// Engine and library definitions
#define GLM_FORCE_RADIANS

// Assertion with error message
#define GIGA_ASSERT(test, message) { if((test) == false) printf(message); assert(test); }

// Ticks per second for the game's systems
#define GIGA_TICKS_PER_SECOND       20

// Windows DLL export definition
#ifndef GIGA_API
	#ifdef GIGA_EXPORTS
		// We are building this library
		#define GIGA_API __declspec(dllexport)
	#else
		// We are using this library
		#define GIGA_API __declspec(dllimport)
	#endif
#endif

// Export for the generator
#ifndef GIGA_CLASS
#define GIGA_FUNCTION(...)
#define GIGA_CLASS(...)
#define GIGA_VARIABLE(...)
#define GIGA_CLASS_BODY(...) void Serialize(DataRecord* record); \
    void Deserialize(DataRecord* record);
#endif

#define FNAME(var) FriendlyName, var

enum ClassOptions {
	Singleton = 1
};

enum VarOptions {
    Get = 1,
    Set,
    Serialize,
	NonEditable,
    Optional
};

// STD library
#include <memory>
#include <mutex>
#include <string>
#include <map>
#include <vector>
#include <algorithm>

// Base math includes; _USE_MATH_DEFINES gives us constants like PI
#define _USE_MATH_DEFINES
#include <cmath>

// Base system includes
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <sys/types.h>
#include <assert.h>

#include <errno.h>
#include <fcntl.h>

// optimize for meta compiler on win32
#ifdef _WIN32
#include <WinSock2.h>
#include <ws2tcpip.h>
#endif
