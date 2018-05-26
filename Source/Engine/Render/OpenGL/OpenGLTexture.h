
#ifndef opengltexture_h
#define opengltexture_h

#include <eternity.h>

class GIGA_API OpenGLTexture {
public:
    OpenGLTexture() : m_texture(0), m_slot(0) { }
    virtual ~OpenGLTexture() = default;
    
protected:
    // Internal texture ID
    unsigned int m_texture;
    
    // Bound texture slot
    int m_slot;
};

#endif
