
#include <Render/OpenGL/OpenGLIndexBuffer.h>
#include <Render/OpenGL/OpenGL.h>

OpenGLIndexBuffer::OpenGLIndexBuffer() {
    m_count = 0;
    m_buffer = 0;
}

OpenGLIndexBuffer::~OpenGLIndexBuffer() {
    
}

void OpenGLIndexBuffer::Create(int count, void *data) {
    m_count = count;
    
    GL_CHECK(glGenBuffers(1, &m_buffer));
    GL_CHECK(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_buffer));
    GL_CHECK(glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(uint32_t) * m_count, data, GL_STATIC_DRAW));
}

void OpenGLIndexBuffer::Destroy() {
    if (m_buffer) {
        GL_CHECK(glDeleteBuffers(1, &m_buffer));
        m_buffer = 0;
        m_count = 0;
    }
}

void OpenGLIndexBuffer::Use() {
    GL_CHECK(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_buffer));
}

uint32_t* OpenGLIndexBuffer::GetData(int& count) {
    uint32_t* indices = (uint32_t*)malloc(sizeof(uint32_t) * m_count);
    GL_CHECK(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_buffer));
    
    int size = 0;
    GL_CHECK(glGetBufferParameteriv(GL_ELEMENT_ARRAY_BUFFER, GL_BUFFER_SIZE, &size));
    GL_CHECK(glGetBufferSubData(GL_ELEMENT_ARRAY_BUFFER, 0, sizeof(uint32_t) * m_count, indices));
    
    count = m_count;
    return(indices);
}
