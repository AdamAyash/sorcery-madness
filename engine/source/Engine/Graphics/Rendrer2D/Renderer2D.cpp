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

	void Renderer2D::Submit(IN const RenderCommand& oRenderCommand)
	{
		m_oRenderCommands.push_back(oRenderCommand);
	}

	void Renderer2D::Draw()
	{
		for (auto& oRenderCommandIterator : m_oRenderCommands)
		{
			const auto& shader = oRenderCommandIterator.pShader;
			if (!shader)
			{
				//TODO LOG
				continue;
			}

			m_oOpenGLQuad.Bind();
			shader->Bind();

			shader->SetMatrix("modelMatrix", oRenderCommandIterator.oTransform.GetModelMatrix());
		}

		m_oRenderCommands.clear();
	}
}