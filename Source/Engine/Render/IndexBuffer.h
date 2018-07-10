
#ifndef indexBuffer_h
#define indexBuffer_h

#include <eternity.h>

/**
 * GPU index buffer
 */
class GIGA_API IndexBuffer {
public:
    IndexBuffer();
    virtual ~IndexBuffer();
    
    /**
     * Create a new buffer and fill it
     */
    virtual void Create(int count, void* data);
    
    /**
     * Get the count of indices in this buffer
     */
    int GetIndexCount() { return m_indexCount; }
    
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
    virtual unsigned int* GetData(int& count);
    
protected:
    // Count of indices
    int m_indexCount;
    unsigned int* m_indexData;
};

#endif
