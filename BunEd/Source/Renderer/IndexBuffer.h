#pragma once

class IndexBuffer
{
public:

	IndexBuffer(const void* const data, uint32_t size);

	~IndexBuffer();

	void Bind() const;
	void Unbind() const;

	unsigned int GetCount() const { return m_Count; }

private:

	unsigned int m_ID = 0;
	unsigned int m_Count = 0;
};
