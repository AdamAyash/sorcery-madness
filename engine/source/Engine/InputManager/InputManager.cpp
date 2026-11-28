#include "InputManager.h"
#include "../Core/Application/Window.h"

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

	bool InputManager::IsKeyPressed(IN const int nKeyCode) const
	{
		return m_oKeys[nKeyCode];
	}

	void InputManager::SetKeyPressed(IN const int nKeyCode, IN const bool bIsPressed)
	{
		m_oKeys[nKeyCode] = bIsPressed;
	}
}