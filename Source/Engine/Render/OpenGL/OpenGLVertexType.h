
#ifndef openglvertextype_h
#define openglvertextype_h

#include <eternity.h>
#include <Render/VertexType.h>

class GIGA_API OpenGLVertexType : public VertexType {
public:
	OpenGLVertexType() = default;
	~OpenGLVertexType() = default;

	void Initialize();
	void Destroy();

	/**
	* Enable a particular vertex attribute (if this vertex type has it)
	*/
	void EnableAttribute(int index, int attrib);
	void DisableAttribute(int index);

	/**
	* Bind/unbind this vertex type
	*/
	void Bind();
	void Unbind();

protected:
	// Internal handle
	unsigned int m_vertexArray;
};

#endif
