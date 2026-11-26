#include "PlayerGameObject.h"

PlayerGameObject::PlayerGameObject()
{
}

PlayerGameObject::~PlayerGameObject()
{
}

void PlayerGameObject::Draw(IN engine::Renderer2D& oRenderer2D)
{
	if (!m_pShader)
		m_pShader = common::CreateScope<engine::Shader>("assets/shaders/VertexShader.glsl", "assets/shaders/FragmentShader.glsl", "exampleShader");

	engine::RenderCommand oRenderCommand;
	oRenderCommand.oTransform = oTransform;
	oRenderCommand.pShader = m_pShader.get();

	oRenderer2D.Submit(oRenderCommand);
}