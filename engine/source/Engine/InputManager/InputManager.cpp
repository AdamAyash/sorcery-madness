#include "InputManager.h"

namespace engine
{
	InputManager::InputManager()
	{
	}

	InputManager::~InputManager()
	{
	}

	InputManager& InputManager::GetInstance()
	{
		static InputManager oInputManager;
		return oInputManager;
	}
}