#include "IndexBuffer.h"

#include "Renderer.h"

IndexBuffer::IndexBuffer(const GLuint* data, GLuint count)
	: m_Count(count)
{
	// Create memory in the graphics card
	GLCall(glGenBuffers(1, &m_RendererID));
	// Makes buffer the current one. Only one at a time.
	GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_RendererID));
	// Fill buffer with data from vert_pos. Static Draw: The data store contents will be modified once and used many times.
	GLCall(glBufferData(GL_ELEMENT_ARRAY_BUFFER, count * sizeof(GLuint), data, GL_STATIC_DRAW));

}

IndexBuffer::~IndexBuffer()
{
	GLCall(glDeleteBuffers(1, &m_RendererID));
}

void IndexBuffer::Bind() const
{
	GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_RendererID));
}

void IndexBuffer::Unbind() const
{
	GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0));
}
