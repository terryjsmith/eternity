
#include <Render/VertexBuffer.h>

VertexBuffer::VertexBuffer() {
	m_count = 0;
	m_type = 0;
	m_dynamic = false;
}

VertexBuffer::~VertexBuffer() {
	if (m_type) {
		delete m_type;
		m_type = 0;
	}
}