#pragma once

#include <Engine/Core/Application/Application.h>

class GameApplication : public engine::Application
{
public:
	GameApplication();
	~GameApplication();

protected:
	bool Initialize() override;
};