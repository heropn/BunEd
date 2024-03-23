#include "pch.h"
#include "VertexBufferLayout.h"

unsigned int LayoutElement::GetSizeOfType() const
{
	switch (m_Type)
	{
	case GL_FLOAT:
		return 4;
	case GL_UNSIGNED_INT:
		return 4;
	case GL_UNSIGNED_BYTE:
		return 1;
	}

	__debugbreak();
	return 0;
}


void VertexBufferLayout::Push(unsigned int count, unsigned int type, bool normalized)
{
	m_Elements.push_back(LayoutElement(count, type, normalized));
	m_Stride += m_Elements.back().GetSizeOfType() * count;
}