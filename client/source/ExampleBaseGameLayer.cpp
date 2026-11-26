#include "ExampleBaseGameLayer.h"
#include <Engine/Graphics/OpenGL/OpenGLQuad.h>

ExampleBaseGameLayer::ExampleBaseGameLayer()
{
}

ExampleBaseGameLayer::~ExampleBaseGameLayer()
{
}

void ExampleBaseGameLayer::Update()
{
}

bool ExampleBaseGameLayer::LoadContent()
{
	AddGameObject<PlayerGameObject>(m_oPlayer);

	m_oPlayer.oTransform.oScale = glm::vec3(0.5, 0.5, 1.0);

	return true;
}