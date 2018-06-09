//
//  OpenGLTexture3D.hpp
//  eternity
//
//  Created by Terry Smith on 2018-06-09.
//  Copyright © 2018 Terry Smith. All rights reserved.
//

#ifndef opengltexture3d_h
#define opengltexture3d_h

#include <eternity.h>
#include <Render/Texture3D.h>
#include <Render/OpenGL/OpenGLTexture.h>

/**
 * Our class for storing 3D (cubemap) textures
 */
class GIGA_API OpenGLTexture3D : public Texture3D, public OpenGLTexture {
public:
    OpenGLTexture3D() = default;
    virtual ~OpenGLTexture3D() = default;
    
    /**
     * Initialize a new "zero" texture
     */
    void Initialize(int width, int height, int format, int type);
    
    /**
     * Set data
     */
    void SetData(int width, int height, int format, int type, void* data) { assert(false); }
    
    /**
     * Get texture info
     */
    unsigned int GetTexture() { return m_texture; }
    
    /**
     * Bind/unbind this texture to a texture slot
     */
    void Bind(int slot);
    void Unbind();
    
    /**
     * Get target (always GL_TEXTURE_2D for 2D textures)
     */
    unsigned int GetTarget(int slot);
    
    /**
     * Dump texture to disk
     */
    void Save(std::string filename);
};


#endif
