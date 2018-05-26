
#ifndef vertextype_h
#define vertextype_h

#include <eternity.h>

enum GIGA_API {
	VERTEXTYPE_ATTRIB_POSITION      = 1,
	VERTEXTYPE_ATTRIB_COLOR         = 1 << 1,
    VERTEXTYPE_ATTRIB_NORMAL        = 1 << 2,
    VERTEXTYPE_ATTRIB_TEXCOORD0     = 1 << 3,
    VERTEXTYPE_ATTRIB_TEXCOORD1     = 1 << 4,
    VERTEXTYPE_ATTRIB_BONES         = 1 << 5,
    VERTEXTYPE_ATTRIB_BONEWEIGHTS   = 1 << 6,
};

class GIGA_API VertexType {
public:
	VertexType() = default;
	virtual ~VertexType();

	virtual void Create() { }
	virtual void Destroy() { }

	/**
	 * Add/remove/check for vertex attribute
	 */
	void AddVertexAttrib(int attrib, int size, int offset);
	void RemoveVertexAttrib(int attrib);
	bool HasVertexAttrib(int attrib);

	int GetVertexSize() { return m_vertexSize; }

	/**
	 * Enable a particular vertex attribute (if this vertex type has it)
	 */
	virtual void EnableAttribute(int index, int attrib) { }
	virtual void DisableAttribute(int index) { }

	/**
	 * Bind/unbind this vertex type
	 */
	virtual void Bind() { }
	virtual void Unbind() { }

	// Internal vertex attribute layout
	struct VertexAttrib {
		int attrib;
		int size;
		int offset;
	};

protected:
	int m_vertexSize;
	int m_attribBitmask;
	std::map<int, VertexAttrib*> m_attribs;
};

#endif
