#include "pch.h"
#include "RenderBuffer.h"

RenderBuffer::RenderBuffer(int32_t width, int32_t height, uint32_t rbFormat)
{
	glGenRenderbuffers(1, &m_ID);
	Bind();
	glRenderbufferStorage(GL_RENDERBUFFER, rbFormat, width, height);
	Unbind();
}

RenderBuffer::~RenderBuffer()
{
	glDeleteRenderbuffers(1, &m_ID);
}

void RenderBuffer::Bind() const
{
	glBindRenderbuffer(GL_RENDERBUFFER, m_ID);
}

void RenderBuffer::Unbind() const
{
	glBindRenderbuffer(GL_RENDERBUFFER, 0);
}
