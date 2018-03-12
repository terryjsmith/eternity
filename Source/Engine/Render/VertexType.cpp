
#include <Render/VertexType.h>

VertexType::~VertexType() {
	std::map<int, VertexAttrib*>::iterator it = m_attribs.begin();
	for (; it != m_attribs.end(); it++) {
		delete it->second;
	}

	m_attribs.clear();
}

void VertexType::AddVertexAttrib(int attrib, int size, int offset) {
	VertexAttrib* a = new VertexAttrib;
	a->attrib = attrib;
	a->offset = offset;
	a->size = size;

	m_attribs[attrib] = a;
	m_vertexSize += size;
	m_attribBitmask |= attrib;
}

void VertexType::RemoveVertexAttrib(int attrib) {
	std::map<int, VertexAttrib*>::iterator it = m_attribs.find(attrib);
	for (; it != m_attribs.end(); it++) {
		if (it->second->attrib == attrib)
			break;
	}

	if (it == m_attribs.end()) {
		// Error
		return;
	}

	m_vertexSize -= it->second->size;
	m_attribs.erase(it);
	m_attribBitmask &= ~attrib;
}

bool VertexType::HasVertexAttrib(int attrib) {
	return(m_attribBitmask & attrib);
}