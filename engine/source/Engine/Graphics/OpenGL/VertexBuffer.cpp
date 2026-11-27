#include "VertexBuffer.h"

namespace engine
{
	VertexBuffer::VertexBufferLayout::VertexBufferLayout(unsigned int nIndex, std::size_t nnStrideSize, int nComponentSize /*= FOUR_COMPONENT_SIZE*/,
		GLenum eComponentType/* = GL_FLOAT*/, GLboolean bNormalized /*= GL_FALSE*/, const void* pCurrentStrideOffset /*= (void*)0*/)
		: m_nIndex(nIndex)
		, m_nnStrideSize(nnStrideSize)
		, m_nComponentSize(nComponentSize)
		, m_eComponentType(eComponentType)
		, m_bNormalized(bNormalized)
		, m_pCurrentStrideOffset(pCurrentStrideOffset)
	{
		Generate();
	}

	VertexBuffer::VertexBufferLayout::~VertexBufferLayout()
	{
	}


	void VertexBuffer::VertexBufferLayout::Generate()
	{
		glVertexAttribPointer(m_nIndex, m_nComponentSize, m_eComponentType, m_bNormalized, m_nnStrideSize, m_pCurrentStrideOffset);
		glEnableVertexAttribArray(m_nIndex);
	}

	void VertexBuffer::VertexBufferLayout::Bind() const
	{
	}

	void VertexBuffer::VertexBufferLayout::UnBind() const
	{
	}

	void VertexBuffer::VertexBufferLayout::Destroy()
	{
	}

	VertexBuffer::VertexBuffer()
		: m_nBufferLayoutCounter(0)
	{
		Generate();
	}

	VertexBuffer::~VertexBuffer()
	{
		Destroy();
	}

	void VertexBuffer::Generate()
	{
		glGenBuffers(1, &m_nObjectID);
	}

	void VertexBuffer::Bind() const
	{
		glBindBuffer(GL_ARRAY_BUFFER, m_nObjectID);
	}

	void VertexBuffer::UnBind() const
	{
		glBindBuffer(GL_ARRAY_BUFFER, 0);
	}

	void VertexBuffer::Destroy()
	{
		m_oVertexBufferLayoutArray.clear();

		if (m_nObjectID != 0)
		{
			glDeleteBuffers(1, &m_nObjectID);
			m_nObjectID = 0;
		}
	}

	void VertexBuffer::AddLayout(IN const std::size_t nnStrideSize, OPTIONAL const  int nComponentSize /*= FOUR_COMPONENT_SIZE*/, OPTIONAL const GLenum eComponentType /*= GL_FLOAT*/,
		OPTIONAL const GLboolean bNormalized /*= GL_FALSE*/, const void* pCurrentStrideOffset/* = (void*)0*/)
	{
		const auto oBufferLayout = common::CreateReference<VertexBufferLayout>(m_nBufferLayoutCounter++, nnStrideSize, nComponentSize, eComponentType, bNormalized, pCurrentStrideOffset);
		m_oVertexBufferLayoutArray.push_back(oBufferLayout);
	}
}