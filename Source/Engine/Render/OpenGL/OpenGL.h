
#ifndef opengl_h
#define opengl_h

#ifdef WIN32

#include <GL/glew.h>

#else

#include <OpenGL/gl3.h>
#include <OpenGL/gl3ext.h>

#endif

#ifdef DEBUG
#define GL_CHECK(stuff) stuff; { GLenum error = glGetError(); if(error) { printf("GL error %d\n", error); assert(error == 0); } }
#else
#define GL_CHECK(stuff) stuff;
#endif

void __stdcall OGLDebugMessage(unsigned int source, unsigned int type, unsigned int id, unsigned int severity, int length, const char* message, void* userParam);

#endif
