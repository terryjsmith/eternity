
#include <Render/IndexBuffer.h>

IndexBuffer::IndexBuffer() {
    m_indexData = 0;
    m_indexCount = 0;
}

IndexBuffer::~IndexBuffer() {
    if(m_indexData) {
        free(m_indexData);
        m_indexData = 0;
    }
}

void IndexBuffer::Create(int count, void* data) {
    m_indexData = (unsigned int*)malloc(sizeof(unsigned int) * count);
    memcpy(m_indexData, data, sizeof(unsigned int) * count);
    m_indexCount = count;
}

unsigned int* IndexBuffer::GetData(int& count) {
    count = m_indexCount;
    return(m_indexData);
}
