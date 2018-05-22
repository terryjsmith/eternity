
#include <IO/MemoryReader.h>

MemoryReader::MemoryReader() {
    m_start = 0;
    m_current = 0;
    m_offset = 0;
    m_size = 0;
}

void MemoryReader::Initialize(unsigned char* block, int bytes) {
    m_start = block;
    m_current = m_start;
    m_size = bytes;
    m_offset = 0;
}

void MemoryReader::Read(void* ptr, int bytes) {
    GIGA_ASSERT(m_offset + bytes <= m_size, "Overflow error.");
    
    //ptr = m_current;
    memcpy(ptr, m_current, bytes);
    m_current += bytes;
    m_offset += bytes;
}
