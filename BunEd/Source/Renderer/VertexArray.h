#pragma once
#include "VertexBuffer.h"
#include "VertexBufferLayout.h"
#include "IndexBuffer.h"

class VertexArray
{
public:

	VertexArray();
	~VertexArray();

	void AddBuffer(const VertexBuffer& vb, const VertexBufferLayout& layout, const IndexBuffer& ib);

	void Bind() const;
	void Unbind() const;

private:

	unsigned int m_ID = 0;
};

