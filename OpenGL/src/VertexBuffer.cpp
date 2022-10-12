#include "VertexBuffer.h"

#include "Renderer.h"

VertexBuffer::VertexBuffer(const void* data, GLuint size)
{
	// Create memory in the graphics card
	GLCall(glGenBuffers(1, &m_RendererID));
	// Makes buffer the current one. Only one at a time.
	GLCall(glBindBuffer(GL_ARRAY_BUFFER, m_RendererID));
	// Fill buffer with data from vert_pos. Static Draw: The data store contents will be modified once and used many times.
	GLCall(glBufferData(GL_ARRAY_BUFFER, size, data, GL_STATIC_DRAW));

}

VertexBuffer::~VertexBuffer()
{
	GLCall(glDeleteBuffers(1, &m_RendererID));
}

void VertexBuffer::Bind() const
{
	GLCall(glBindBuffer(GL_ARRAY_BUFFER, m_RendererID));
}

void VertexBuffer::Unbind() const
{
	GLCall(glBindBuffer(GL_ARRAY_BUFFER, 0));
}
