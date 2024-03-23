#pragma once

#include <vector>

struct LayoutElement
{
	LayoutElement(unsigned int count, unsigned int type, bool normalized)
		: m_Count(count), m_Type(type), m_Normalized(normalized) { }

	unsigned int m_Count;
	unsigned int m_Type;
	bool m_Normalized;

	unsigned int GetSizeOfType() const;
};

class VertexBufferLayout
{
public:

	VertexBufferLayout() = default;

	void Push(unsigned int count, unsigned int type, bool normalized);

	inline const std::vector<LayoutElement>& GetElements() const { return m_Elements; }
	inline const unsigned int GetStride() const { return m_Stride; }

private:

	std::vector<LayoutElement> m_Elements;
	unsigned int m_Stride = 0;;
};