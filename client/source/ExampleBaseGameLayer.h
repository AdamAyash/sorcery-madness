#pragma once

#include <Common/Common.h>
#include <Engine/Layers/BaseGameLayer.h>
#include <Engine/Graphics/OpenGL/Shader.h>
#include "Client/PlayerGameObject.h"

class ExampleBaseGameLayer : public engine::BaseGameLayer
{
public:
	ExampleBaseGameLayer();
	~ExampleBaseGameLayer();

public:
	 void Update() override;
	 void Draw() override;

private:
	bool LoadContent() override;

private:
	common::Scope<engine::Shader> m_pShader = nullptr;
	PlayerGameObject m_oPlayer;
};