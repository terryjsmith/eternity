
#include <Render/VertexBuffer.h>

VertexBuffer::VertexBuffer() {
	m_count = 0;
	m_type = 0;
	m_dynamic = false;
    m_vertexData = 0;
    m_vertexCount = 0;
}

VertexBuffer::~VertexBuffer() {
	if (m_type) {
		delete m_type;
		m_type = 0;
	}
    
    if(m_vertexData) {
        free(m_vertexData);
        m_vertexData = 0;
        m_vertexCount = 0;
    }
}

void VertexBuffer::Create(VertexType* type, int count, float* data, bool dynamic) {
    m_vertexData = (float*)malloc(sizeof(float) * type->GetVertexSize() * count);
    memcpy(m_vertexData, data, sizeof(float) * type->GetVertexSize() * count);
    m_vertexCount = count;
}

float* VertexBuffer::GetData(int& count) {
    count = m_vertexCount;
    return(m_vertexData);
}
