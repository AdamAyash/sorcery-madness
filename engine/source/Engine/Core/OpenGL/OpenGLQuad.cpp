#include "OpenGLQuad.h"
#include <glad/glad.h>

namespace engine
{
	OpenGLQuad::OpenGLQuad()
	{
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

		auto nStrideOffeset = sizeof(float) * 2;

		m_oVertexBuffer.Pack<float>(oVertices, GL_STATIC_DRAW);
		m_oVertexBuffer.AddLayout(nStrideOffeset, TWO_COMPONENT_SIZE);
	}

	void OpenGLQuad::Bind() const
	{
		m_oVertexBuffer.Bind();
	}

	void OpenGLQuad::UnBind() const
	{
		m_oVertexBuffer.UnBind();
	}

	void OpenGLQuad::Destroy()
	{
	}
}