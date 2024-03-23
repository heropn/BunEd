#pragma once

class VertexBuffer
{
public:

	VertexBuffer(const void* const data, unsigned int size);

	VertexBuffer(const VertexBuffer&) = delete;
	VertexBuffer& operator=(const VertexBuffer&) = delete;

	~VertexBuffer();

	void Bind() const;
	void Unbind() const;

private:

	uint32_t m_ID = 0;
};
