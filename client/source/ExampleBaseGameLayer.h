#pragma once

#include <Common/Common.h>
#include <Engine/Layers/BaseGameLayer.h>
#include <Engine/Graphics/OpenGL/Shader.h>
#include <Engine/Graphics/Rendrer2D/Renderer2D.h>
#include "Client/PlayerGameObject.h"

class ExampleBaseGameLayer : public engine::BaseGameLayer
{
public:
	ExampleBaseGameLayer();
	~ExampleBaseGameLayer();

public:
	 void Update() override;
	 void Draw(IN engine::Renderer2D& oRenderer2D) override;

private:
	bool LoadContent() override;

private:
	common::Scope<engine::Shader> m_pShader = nullptr;
	PlayerGameObject m_oPlayer;
};