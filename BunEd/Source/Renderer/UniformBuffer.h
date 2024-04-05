#pragma once

class UniformBuffer
{
public:

	UniformBuffer(const void* const data, const uint32_t size);
	~UniformBuffer();

	void Bind(uint32_t index);
	void Unbind();

	void UpdateBufferData(const int32_t offset, const uint32_t size, const void* const data);

private:
	
	uint32_t m_BoundIndex = -1;
	uint32_t m_ID = 0;
};
