#pragma once

class IndexBuffer
{
public:

	IndexBuffer(const void* const data, uint16_t indices);

	~IndexBuffer();

	void Bind() const;
	void Unbind() const;

	unsigned int GetCount() const { return m_Count; }

private:

	unsigned int m_ID = 0;
	unsigned int m_Count = 0;
};
