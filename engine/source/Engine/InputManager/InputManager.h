#pragma once

#include <Common/Common.h>

#define KEYBOARD_KEY_ARRAY_SIZE 256

namespace engine
{
	class Window;

	class InputManager : public common::NonCopyableObject
	{
	private:
		InputManager();

	public:
		~InputManager();

	public:
		static InputManager& GetInstance();
		bool IsKeyPressed(IN const int nKeyCode) const;

	private:
		void SetKeyPressed(IN const int nKeyCode, IN const bool bIsPressed);

	private:
		std::array<bool, KEYBOARD_KEY_ARRAY_SIZE> m_oKeys{ false };

		friend Window;
	};
}