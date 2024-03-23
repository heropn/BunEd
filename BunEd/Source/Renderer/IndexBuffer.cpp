#include "pch.h"
#include "IndexBuffer.h"

IndexBuffer::IndexBuffer(const void* const indices, unsigned int size)
	: m_Size(size / sizeof(unsigned int))
{
	glGenBuffers(1, &m_ID);
	Bind();
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, size, indices, GL_STATIC_DRAW);
	Unbind();
}

IndexBuffer::~IndexBuffer()
{
	glDeleteBuffers(1, &m_ID);
}

void IndexBuffer::Bind() const
{
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_ID);
}

void IndexBuffer::Unbind() const
{
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}