#pragma once

#include <Engine/Application/Application.h>
#include <Engine//Graphics/Shader.h>


class GameApplication : public engine::Application
{
public:
	GameApplication();
	~GameApplication();

public:
	bool Initialize() override;
	void Draw() override;

private:
	engine::Shader m_oShader;
};