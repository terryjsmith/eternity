
#ifndef texture2d_h
#define texture2d_h

#include <eternity.h>
#include <Render/Texture.h>

/**
 * Our class for storing textures loaded from disk
 */
class GIGA_API Texture2D : public Texture {
public:
    Texture2D() = default;
    ~Texture2D() = default;
    
    GIGA_CLASS_NAME("Texture2D");
};

#endif
