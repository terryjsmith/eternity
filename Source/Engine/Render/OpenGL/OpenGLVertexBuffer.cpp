
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