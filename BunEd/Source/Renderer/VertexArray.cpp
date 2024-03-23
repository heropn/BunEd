#include "pch.h"
#include "VertexArray.h"

VertexArray::VertexArray()
{
	glGenVertexArrays(1, &m_ID);
}

VertexArray::~VertexArray()
{
	glDeleteVertexArrays(1, &m_ID);
}

void VertexArray::AddBuffer(const VertexBuffer& vb, const VertexBufferLayout& layout, const IndexBuffer& ib)
{
	Bind();
	vb.Bind();
	ib.Bind();

	const std::vector<LayoutElement>& layoutElementes = layout.GetElements();
	unsigned int offset = 0;

	for (int i = 0; i < layoutElementes.size(); i++)
	{
		const LayoutElement& el = layoutElementes[i];

		glEnableVertexAttribArray(i);
		glVertexAttribPointer(i, el.m_Count, el.m_Type, el.m_Normalized, layout.GetStride(), (const void*)offset);
		offset += el.m_Count * el.GetSizeOfType();
	}

	Unbind();
}

void VertexArray::Bind() const
{
	glBindVertexArray(m_ID);
}

void VertexArray::Unbind() const
{
	glBindVertexArray(0);
}
