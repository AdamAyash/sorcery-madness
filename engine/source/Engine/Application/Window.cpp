#include "Window.h"
#include <iostream>
#include "../OpenGL/OpenGLContext.h"
#include <Common/Assert.h>

namespace engine
{
	Window::Window(IN const WindowProperties& oWindowProperties)
	{
		Initialize(oWindowProperties);
	}

	Window::~Window()
	{
		ShutDown();
	}

	void Window::Initialize(IN const WindowProperties& oWindowProperties)
	{
		OpenGLContext oOpenGLContext;
		if (!oOpenGLContext.InitializeOpenGLContext())
		{
			LOG_ERROR("Failed to initalize OpenGL context.");
		}

		m_pWindow = glfwCreateWindow(oWindowProperties.m_nWidth, oWindowProperties.m_nHeight,
			oWindowProperties.m_strTitle.c_str()
			, nullptr, nullptr);

		ASSERT(m_pWindow);

		LOG_INFO("Creating window {0} ({1}, {2})", oWindowProperties.m_strTitle, oWindowProperties.m_nWidth, oWindowProperties.m_nHeight);

		glfwMakeContextCurrent(m_pWindow);
		glfwSetWindowUserPointer(m_pWindow, reinterpret_cast<void*>(this));

		oOpenGLContext.InitializeGlad();
		oOpenGLContext.DisplayOpenGLContextInfo();

		/*glfwSetWindowCloseCallback(m_pWindow, [](GLFWwindow* pWindow)
			{
				const Window& oWindow = *reinterpret_cast<const Window*>(glfwGetWindowUserPointer(pWindow));
				WindowCloseEvent oWindowCloseEvent;
				oWindow.m_oEventListener->OnEvent(oWindowCloseEvent);
			});

		glfwSetFramebufferSizeCallback(m_pWindow, [](GLFWwindow* pWindow, int nWidth, int nHeight)
			{
				glViewport(0, 0, nWidth, nHeight);

				const Window& oWindow = *reinterpret_cast<const Window*>(glfwGetWindowUserPointer(pWindow));

				WindowResizeEvent oWindowResizeEvent(nWidth, nHeight);
				oWindow.m_oEventListener->OnEvent(oWindowResizeEvent);
			});*/
	}

	void Window::ShutDown()
	{
		glfwDestroyWindow(m_pWindow);
		glfwTerminate();
	}

	void Window::OnUpdate()
	{
		glfwPollEvents();
		glfwSwapBuffers(m_pWindow);
	}

	constexpr GLFWwindow* Window::GetNativeWindow() const
	{
		return m_pWindow;
	}
}