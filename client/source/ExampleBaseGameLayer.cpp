#include "ExampleBaseGameLayer.h"
#include <Engine/Core/OpenGL/OpenGLQuad.h>

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
	engine::OpenGLQuad opengGLQuad;
	opengGLQuad.Bind();

	glDrawArrays(GL_TRIANGLES, 0, 6);
}

void ExampleBaseGameLayer::LoadContent()
{
}