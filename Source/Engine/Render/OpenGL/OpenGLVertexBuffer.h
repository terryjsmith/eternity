
#ifndef openglvertexbuffer_h
#define openglvertexbuffer_h

#include <eternity.h>
#include <Render/VertexBuffer.h>

class GIGA_API OpenGLVertexBuffer : public VertexBuffer {
public:
	OpenGLVertexBuffer() = default;
	~OpenGLVertexBuffer() = default;

	void Create(VertexType* type, int count, float* data, bool dynamic);
	void Bind();
	void Unbind();
    
    /**
     * Read the buffer's contents back out
     */
    float* GetData(int& count);

protected:
	// internal buffer
	unsigned int m_buffer;
};

#endif
