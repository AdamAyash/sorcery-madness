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

	if (!PushLayer(new ExampleBaseGameLayer()))
	{
		return false;
	}

	return true;
}