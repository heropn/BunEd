#pragma once

class IndexBuffer
{
public:

	IndexBuffer(const void* const data, unsigned int indices);

	~IndexBuffer();

	void Bind() const;
	void Unbind() const;

	unsigned int GetSize() const { return m_Size; }

private:

	unsigned int m_ID = 0;
	unsigned int m_Size = 0;
};
