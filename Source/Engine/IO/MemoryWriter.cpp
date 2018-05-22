
#include <IO/MemoryWriter.h>

MemoryWriter::MemoryWriter() {
    m_start = 0;
    m_current = 0;
    m_offset = 0;
    m_size = 0;
}

void MemoryWriter::Initialize(unsigned char* block, int bytes) {
    m_start = block;
    m_current = m_start;
    m_size = bytes;
    m_offset = 0;
}

void MemoryWriter::Write(void* ptr, int bytes) {
    GIGA_ASSERT(m_offset + bytes <= m_size, "Overflow error.");
    
    memcpy(m_current, ptr, bytes);
    m_current += bytes;
    m_offset += bytes;
}
