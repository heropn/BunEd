#pragma once

class IndexBuffer
{
public:

	IndexBuffer(const void* const data, uint32_t size);

	~IndexBuffer();

	void Bind() const;
	void Unbind() const;

	uint32_t GetCount() const { return m_Count; }

private:

	uint32_t m_ID = 0;
	uint32_t m_Count = 0;
};
