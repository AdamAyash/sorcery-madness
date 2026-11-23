#include "GameApplication.h"
#include "ExampleBaseGameLayer.h"

GameApplication::GameApplication()
{
}

GameApplication::~GameApplication()
{
}

bool GameApplication::Initialize()
{
	if (!BASE::Initialize())
	{
		//TODO ERROR.
		return false;
	}

	PushLayer(new ExampleBaseGameLayer());

	return true;
}