#include "pch.h"
#include "UniformBuffer.h"

UniformBuffer::UniformBuffer(const void* const data, const uint32_t size)
{
	glGenBuffers(1, &m_ID);
	glBindBuffer(GL_UNIFORM_BUFFER, m_ID);
	glBufferData(GL_UNIFORM_BUFFER, size, data, GL_DYNAMIC_DRAW);
	glBindBuffer(GL_UNIFORM_BUFFER, 0);
}

UniformBuffer::~UniformBuffer()
{
	glDeleteBuffers(1, &m_ID);
}

void UniformBuffer::Bind(uint32_t index)
{
	m_BoundIndex = index;
	glBindBufferBase(GL_UNIFORM_BUFFER, m_BoundIndex, m_ID);
}

void UniformBuffer::Unbind()
{
	glBindBufferBase(GL_UNIFORM_BUFFER, m_BoundIndex, 0);
	m_BoundIndex = -1;
}

void UniformBuffer::UpdateBufferData(const int32_t offset, const uint32_t size, const void* const data)
{
	glBindBuffer(GL_UNIFORM_BUFFER, m_ID);
	glBufferSubData(GL_UNIFORM_BUFFER, offset, size, data);
	glBindBuffer(GL_UNIFORM_BUFFER, 0);
}
