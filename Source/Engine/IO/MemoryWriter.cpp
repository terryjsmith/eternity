
#include <IO/MemoryWriter.h>

MemoryWriter::MemoryWriter() {
    m_start = 0;
    m_current = 0;
    m_offset = 0;
    m_size = 0;
    m_chunkSize = 0;
    m_resizable = false;
}

void MemoryWriter::Initialize(unsigned char* block, int bytes) {
    m_start = block;
    m_current = m_start;
    m_size = bytes;
    m_offset = 0;
}

void MemoryWriter::Write(void* ptr, int bytes) {
    if(m_resizable == false) {
        GIGA_ASSERT(m_offset + bytes <= m_size, "Overflow error.");
    }
    else {
        if(m_offset + bytes >= m_size) {
            unsigned char* newbytes = (unsigned char*)malloc(m_size + m_chunkSize);
            memcpy(newbytes, m_start, m_offset);
            free(m_start);
            
            m_start = newbytes;
            m_current = newbytes + m_offset;
            m_size = m_size + m_chunkSize;
        }
    }
    
    memcpy(m_current, ptr, bytes);
    m_current += bytes;
    m_offset += bytes;
}

void MemoryWriter::Resizable(bool resize, int chunkSize) {
    m_chunkSize = chunkSize;
    m_resizable = resize;
}
