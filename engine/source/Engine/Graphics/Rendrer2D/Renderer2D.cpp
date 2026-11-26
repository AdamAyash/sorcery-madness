#include "Renderer2D.h"
#include "../../Core/Application/Application.h"

namespace engine
{
	Renderer2D::Renderer2D()
	{
	}

	Renderer2D::~Renderer2D()
	{
	}

	void Renderer2D::Submit(IN const RenderCommand &oRenderCommand)
	{
		m_oRenderCommands.push_back(oRenderCommand);
	}

	void Renderer2D::Draw()
	{
		glClearColor(0.1f, 0.1f, 0.15f, 1.0f); // TODO REMOVE
		glClear(GL_COLOR_BUFFER_BIT);

		m_oProjectionMatrix = glm::mat4(1);
		m_oProjectionMatrix = glm::ortho(0.0, 1600.0, 0.0, 900.0);

		for (auto &oRenderCommandIterator : m_oRenderCommands)
		{
			const auto &shader = oRenderCommandIterator.pShader;
			if (!shader)
			{
				// TODO LOG
				continue;
			}

			m_oOpenGLQuad.Bind();

			shader->Bind();
			shader->SetMatrix("modelMatrix", oRenderCommandIterator.oTransform.GetModelMatrix());
			shader->SetMatrix("projectionMatrix", m_oProjectionMatrix);

			glDrawArrays(GL_TRIANGLES, 0, 6);

			m_oOpenGLQuad.UnBind();
		}

		m_oRenderCommands.clear();
	}
}