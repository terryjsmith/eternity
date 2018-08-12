
#ifndef memorywriter_h
#define memorywriter_h

#include <eternity.h>

/**
 * Utility class for writing bytes to of a continuous block in chunks
 */
class GIGA_API MemoryWriter {
public:
    MemoryWriter();
    ~MemoryWriter() = default;
    
    /**
     * Initialize a new memory reader
     */
    void Initialize(unsigned char* block, int bytes);
    
    /**
     * Allow dynamic resizing
     */
    void Resizable(bool resize, int chunkSize = 0);
    
    /**
     * Write some data
     */
    void Write(void* ptr, int bytes);
    
    /**
     * Get current offset into the data
     */
    int GetPosition() { return m_offset; }
    
protected:
    // Starting pointer
    unsigned char* m_start;
    
    // Current location
    unsigned char* m_current;
    
    // Offset (in bytes)
    int m_offset;
    
    // Total size
    int m_size;
    
    // Resize chunk size
    int m_chunkSize;
    
    // Resizable
    bool m_resizable;
};


#endif
