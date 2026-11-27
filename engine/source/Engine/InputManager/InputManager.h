#pragma once

#include <Common/Common.h>

namespace engine
{
	class InputManager : public common::NonCopyableObject
	{
	private:
		InputManager();

	public:
		~InputManager();

	public:
		static InputManager& GetInstance();
	};
}