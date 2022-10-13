#pragma once

#include "VertexBuffer.h"

class VertexArray
{
private:

public:
	VertexArray();
	~VertexArray();
	
	void AddBuffer(const VertexBuffer& vb, const VertexBufferLayout& layout);

};
