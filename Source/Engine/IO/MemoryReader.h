
#ifndef memoryreader_hpp
#define memoryreader_hpp

#include <eternity.h>

/**
 * Utility class for reading bytes out of a continuous block in chunks
 */
class GIGA_API MemoryReader {
public:
    MemoryReader();
    ~MemoryReader() = default;
    
    /**
     * Initialize a new memory reader
     */
    void Initialize(unsigned char* block, int bytes);
    
    /**
     * Read some data
     */
    void Read(void* ptr, int bytes);
    
    /**
     * Get current offset into the data
     */
    int GetPosition() { return m_offset; }
    
    /**
     * Set position
     */
    void SetPosition(int position) { m_offset = position; m_current = m_start + m_offset; }
    
    /**
     * Get current position
     */
    unsigned char* GetCurrent() { return m_current; }
    
protected:
    // Starting pointer
    unsigned char* m_start;
    
    // Current location
    unsigned char* m_current;
    
    // Offset (in bytes)
    int m_offset;
    
    // Total size
    int m_size;
};

#endif
