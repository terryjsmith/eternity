
#include <Render/OpenGL/OpenGLVertexBuffer.h>
#include <Render/OpenGL/OpenGL.h>

void OpenGLVertexBuffer::Create(VertexType* type, int count, float* data, bool dynamic) {
	m_count = count;
	m_type = type;
	m_dynamic = dynamic;

	GL_CHECK(glGenBuffers(1, &m_buffer));
	GL_CHECK(glBindBuffer(GL_ARRAY_BUFFER, m_buffer));
	GL_CHECK(glBufferData(GL_ARRAY_BUFFER, sizeof(float) * type->GetVertexSize() * m_count, data, dynamic ? GL_STREAM_DRAW : GL_STATIC_DRAW));
}

void OpenGLVertexBuffer::Bind() {
	GL_CHECK(glBindBuffer(GL_ARRAY_BUFFER, m_buffer));

	int size = 0;
	GL_CHECK(glGetBufferParameteriv(GL_ARRAY_BUFFER, GL_BUFFER_SIZE, &size));
}

void OpenGLVertexBuffer::Unbind() {
	GL_CHECK(glBindBuffer(GL_ARRAY_BUFFER, 0));
}

float* OpenGLVertexBuffer::GetData(int &count) {
    std::vector<float> vertices;
    int vertexSize = m_type->GetVertexSize();
    vertices.resize(m_count * vertexSize);
    
    float* ret = (float*)malloc(m_count * 3 * sizeof(float));
    GL_CHECK(glBindBuffer(GL_ARRAY_BUFFER, m_buffer));
    
    int size = 0;
    GL_CHECK(glGetBufferParameteriv(GL_ARRAY_BUFFER, GL_BUFFER_SIZE, &size));
    GL_CHECK(glGetBufferSubData(GL_ARRAY_BUFFER, 0, vertexSize * m_count * sizeof(float), vertices.data()));
    
    for (int i = 0; i < m_count; i++) {
        int offset = i * 3;
        
        ret[offset + 0] = vertices[i * vertexSize + 0];
        ret[offset + 1] = vertices[i * vertexSize + 1];
        ret[offset + 2] = vertices[i * vertexSize + 2];
    }
    
    count = m_count;
    return(ret);
}
