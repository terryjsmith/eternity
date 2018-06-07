
#ifndef indexBuffer_h
#define indexBuffer_h

#include <eternity.h>

/**
 * GPU index buffer
 */
class GIGA_API IndexBuffer {
public:
    IndexBuffer() : m_count(0) { }
    virtual ~IndexBuffer() = default;
    
    /**
     * Create a new buffer and fill it
     */
    virtual void Create(int count, void* data) { }
    
    /**
     * Get the count of indices in this buffer
     */
    int GetIndexCount() { return m_count; }
    
    /**
     * Use this index buffer
     */
    virtual void Bind() { }
    
    /**
     * Destroy this buffer's contents
     */
    virtual void Destroy() { }
    
    /**
     * Read the buffer's contents
     */
    virtual unsigned int* GetData(int& count) { count = 0; return(0); }
    
protected:
    // Count of indices
    int m_count;
};

#endif
