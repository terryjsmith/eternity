
#ifndef openglindexbuffer_hpp
#define openglindexbuffer_hpp

#include <eternity.h>
#include <Render/IndexBuffer.h>

/**
 * GPU index buffer
 */
class GIGA_API OpenGLIndexBuffer : public IndexBuffer {
public:
    OpenGLIndexBuffer();
    ~OpenGLIndexBuffer();
    
    /**
     * Create a new buffer and fill it
     */
    void Create(int count, void* data);
    
    /**
     * Use this index buffer
     */
    void Use();
    
    /**
     * Destroy this buffer's contents
     */
    void Destroy();
    
    /**
     * Read the buffer's contents
     */
    unsigned int* GetData(int& count);
    
protected:
    // Internal buffer ID
    unsigned int m_buffer;
};

#endif
