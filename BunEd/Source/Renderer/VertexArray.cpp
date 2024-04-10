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

	for (; m_AttribCount < layoutElementes.size(); m_AttribCount++)
	{
		const LayoutElement& el = layoutElementes[m_AttribCount];

		glEnableVertexAttribArray(m_AttribCount);
		glVertexAttribPointer(m_AttribCount, el.m_Count, el.m_Type, el.m_Normalized, layout.GetStride(), (const void*)offset);
		offset += el.m_Count * el.GetSizeOfType();
	}

	Unbind();
}

void VertexArray::AddInstancedArrayBuffer(const VertexBuffer& vb, const VertexBufferLayout& layout)
{
	Bind();
	vb.Bind();

	const std::vector<LayoutElement>& layoutElementes = layout.GetElements();
	unsigned int offset = 0;

	for (; m_AttribCount < layoutElementes.size(); m_AttribCount++)
	{
		const LayoutElement& el = layoutElementes[m_AttribCount];

		glEnableVertexAttribArray(m_AttribCount);
		glVertexAttribPointer(m_AttribCount, el.m_Count, el.m_Type, el.m_Normalized, layout.GetStride(), (const void*)offset);
		glVertexAttribDivisor(m_AttribCount, 1);
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
