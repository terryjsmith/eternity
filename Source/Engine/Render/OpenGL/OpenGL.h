
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

// Color mapping
int COLOR_RGB = GL_RGB;
int COLOR_RGBA = GL_RGBA;
int COLOR_RGB16F = GL_RGB16F;
int COLOR_RED = GL_RED;
int COLOR_RED16F = GL_R16F;
int COLOR_RED32F = GL_R32F;
int COLOR_DEPTH_COMPONENT32 = GL_DEPTH_COMPONENT32;
int COLOR_DEPTH_COMPONENT = GL_DEPTH_COMPONENT;

#endif
