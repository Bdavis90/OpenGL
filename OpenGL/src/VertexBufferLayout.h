#pragma once

#include <vector>
#include "GL/glew.h"

struct VertexBufferElement 
{
	GLuint type;
	GLuint count;
	bool normalized;
};


class VertexBufferLayout
{
private:
	std::vector<VertexBufferElement> m_Elements;
	GLuint m_Stride;
public:

	VertexBufferLayout()
		: m_Stride(0) {};

	template<typename T>
	void Push(int count)
	{
		static_assert(false);
	}

	template <>
	void Push<GLfloat>(GLint count)
	{
		m_Elements.push_back(VertexBufferElement{ GL_FLOAT, (GLuint)count, false });
		m_Stride += sizeof(GLfloat);
	};

	template <>
	void Push<GLuint>(GLint count)
	{
		m_Elements.push_back(VertexBufferElement{ GL_UNSIGNED_INT, (GLuint)count, false });
		m_Stride += sizeof(GLuint);
	};

	template <>
	void Push<GLchar>(GLint count)
	{
		m_Elements.push_back(VertexBufferElement{ GL_UNSIGNED_BYTE, (GLuint)count, true });
		m_Stride += sizeof(GLbyte);
	};

	inline const std::vector<VertexBufferElement>& GetElements() const { return m_Elements; };
	inline GLuint GetStride() const { return m_Stride; };
};