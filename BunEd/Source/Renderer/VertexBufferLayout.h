#pragma once

#include <vector>

struct LayoutElement
{
	LayoutElement(uint32_t count, uint32_t type, bool normalized)
		: m_Count(count), m_Type(type), m_Normalized(normalized) { }

	uint32_t m_Count;
	uint32_t m_Type;
	bool m_Normalized;

	uint32_t GetSizeOfType() const;
};

class VertexBufferLayout
{
public:

	VertexBufferLayout() = default;

	void Push(uint32_t count, uint32_t type, bool normalized);

	inline const std::vector<LayoutElement>& GetElements() const { return m_Elements; }
	inline const uint32_t GetStride() const { return m_Stride; }

private:

	std::vector<LayoutElement> m_Elements;
	uint32_t m_Stride = 0;;
};