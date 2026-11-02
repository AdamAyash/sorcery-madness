#include "Engine.h"
#include "Core/OpenGLContext.h"

namespace engine
{
	Engine& Engine::GetInstance()
	{
		static Engine oEngineInstance;
		return oEngineInstance;
	}

	bool Engine::Initialize()
	{
		OpenGLContext oOpenGLContext;
		if (!oOpenGLContext.InitializeOpenGLContext())
		{
			return false;
		}

		m_pWindow = glfwCreateWindow(1280, 720, "", nullptr, nullptr);
		glfwMakeContextCurrent(m_pWindow);

		if (!m_pWindow)
		{
			glfwTerminate();
			return false;
		}

		if (!oOpenGLContext.InitializeGlad())
		{
			return false;
		}

		oOpenGLContext.DisplayOpenGLContextInfo();

		return true;
	}

	void Engine::Run()
	{
		while (!glfwWindowShouldClose(m_pWindow))
		{

			glfwPollEvents();
			glfwSwapBuffers(m_pWindow);
		}
	}
}