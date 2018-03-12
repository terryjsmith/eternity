
#include <Render/OpenGL/OpenGLVertexType.h>
#include <Render/OpenGL/OpenGL.h>

void OpenGLVertexType::Create() {
	GL_CHECK(glGenVertexArrays(1, &m_vertexArray));
}

void OpenGLVertexType::Destroy() {
	if (m_vertexArray) {
		GL_CHECK(glDeleteVertexArrays(1, &m_vertexArray));
	}
}

void OpenGLVertexType::EnableAttribute(int index, int attrib) {
	if ((m_attribBitmask & attrib) == 0) {
		return;
	}

	GL_CHECK(glEnableVertexAttribArray(index));
	GL_CHECK(glVertexAttribPointer(index, m_attribs[attrib]->size, GL_FLOAT, GL_FALSE, sizeof(float) *  m_vertexSize, (void*)(sizeof(float) * m_attribs[attrib]->offset)));
}

void OpenGLVertexType::DisableAttribute(int index) {
	GL_CHECK(glDisableVertexAttribArray(index));
}

void OpenGLVertexType::Bind() {
	GL_CHECK(glBindVertexArray(m_vertexArray));
}

void OpenGLVertexType::Unbind() {
	GL_CHECK(glBindVertexArray(0));
}