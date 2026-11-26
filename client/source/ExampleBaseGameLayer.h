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

private:
	bool LoadContent() override;

private:
	PlayerGameObject m_oPlayer;
};