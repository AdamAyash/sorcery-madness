#include "GameTime.h"
#include <GLFW/glfw3.h>

namespace engine
{
	GameTime::GameTime()
	{
	}

	GameTime::~GameTime()
	{
	}

	void GameTime::Calculate()
	{
		m_dCurrentTime = glfwGetTime();
		m_dDeltaTime = m_dCurrentTime - m_dLastFrame;

		m_dLastFrame = m_dCurrentTime;
	}

	double GameTime::GetDeltaTime() const
	{
		return m_dDeltaTime;
	}

	double GameTime::GetTotalGameTime() const
	{
		return m_dCurrentTime;
	}
}