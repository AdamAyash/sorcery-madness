#pragma once

#include <Common/Common.h>
#include <Engine/Layers/BaseGameLayer.h>
#include <Engine/Graphics/OpenGL/Shader.h>
#include <Engine/Graphics/Renderer2D/Renderer2D.h>
#include "Client/PlayerGameObject.h"
#include <Engine/EventSystem/Event.h>

class ExampleBaseGameLayer : public engine::BaseGameLayer
{
public:
	ExampleBaseGameLayer();
	~ExampleBaseGameLayer();
	
public:
	void OnEvent(engine::events::Event& oEvent) override;
	 void OnUpdate() override;

private:
	bool LoadContent() override;

private:
	PlayerGameObject m_oPlayer;
};