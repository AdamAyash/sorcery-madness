#include "OpenGLQuad.h"
#include <glad/glad.h>

namespace engine
{
	OpenGLQuad::OpenGLQuad()
	{
		Generate();
	}

	OpenGLQuad::~OpenGLQuad()
	{
		Destroy();
	}

	void OpenGLQuad::Generate()
	{
		std::vector<float> oVertices =
		{
			// pos     
			0.0f, 1.0f,
			1.0f, 0.0f,
			0.0f, 0.0f,

			0.0f, 1.0f,
			1.0f, 1.0f,
			1.0f, 0.0f,
		};

		m_pVertexArray = common::CreateScope<VertexArray>();
		m_pVertexArray->Bind();

		const auto nStrideOffeset = sizeof(float) * 2;

		auto pVertexBuffer  = common::CreateReference<VertexBuffer>();
		pVertexBuffer->Bind();
		pVertexBuffer->Pack<float>(oVertices, GL_STATIC_DRAW);
		pVertexBuffer->AddLayout(nStrideOffeset, TWO_COMPONENT_SIZE);

		m_pVertexArray->AddVertexBuffer(pVertexBuffer);
		m_pVertexArray->UnBind();
	}

	void OpenGLQuad::Bind() const
	{
		m_pVertexArray->Bind();
	}

	void OpenGLQuad::UnBind() const
	{
		m_pVertexArray->UnBind();
	}

	void OpenGLQuad::Destroy()
	{
	}
}