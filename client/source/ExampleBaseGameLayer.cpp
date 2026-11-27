#include "ExampleBaseGameLayer.h"
#include <Engine/Graphics/OpenGL/OpenGLQuad.h>

ExampleBaseGameLayer::ExampleBaseGameLayer()
{
}

ExampleBaseGameLayer::~ExampleBaseGameLayer()
{
}

void ExampleBaseGameLayer::OnEvent(engine::events::Event& oEvent)
{
}

void ExampleBaseGameLayer::OnUpdate()
{
}

bool ExampleBaseGameLayer::LoadContent()
{
	AddGameObject<PlayerGameObject>(m_oPlayer);

	m_oPlayer.oTransform.oPosition.x = 500;
	m_oPlayer.oTransform.oPosition.y = 500;
	m_oPlayer.oTransform.oScale = glm::vec3(16, 16, 1.0);

	return true;
}