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

void ExampleBaseGameLayer::Draw()
{
	glClearColor(0.1f, 0.1f, 0.15f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT);

	glDrawArrays(GL_TRIANGLES, 0, 6);
}

bool ExampleBaseGameLayer::LoadContent()
{
	m_pShader = common::CreateScope<engine::Shader>("assets/shaders/VertexShader.glsl", "assets/shaders/FragmentShader.glsl", "exampleShader");

	if (!m_pShader->IsShaderValid())
		return false;

	AddGameObject<PlayerGameObject>(m_oPlayer);

	m_oPlayer.oTransform.oScale = glm::vec3(0.5, 0.5, 1.0);

	return true;
}