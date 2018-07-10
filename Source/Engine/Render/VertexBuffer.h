
#ifndef vertexbuffer_h
#define vertexbuffer_h

#include <eternity.h>
#include <Render/VertexType.h>

class GIGA_API VertexBuffer {
public:
	VertexBuffer();
	virtual ~VertexBuffer();

    virtual void Create(VertexType* type, int count, float* data, bool dynamic);
	virtual void Bind() { }
	virtual void Unbind() { }

	VertexType* GetType() { return m_type; }
	int GetCount() { return m_count; }
    
    /**
     * Read the buffer's contents back out
     */
    virtual float* GetData(int& count);

protected:
	int m_count;
	VertexType* m_type;
	bool m_dynamic;
    
    float* m_vertexData;
    int m_vertexCount;
};

#endif
