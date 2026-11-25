#include "Window.h"
#include "../../Graphics/OpenGL/OpenGLContext.h"
#include "../../EventSystem/Events/WindowEvents/WindowCloseEvent.h"
#include "../../EventSystem/EventDispatcher.h"

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

		const bool bIsGladInitialized = oOpenGLContext.InitializeGlad();
		ASSERT(bIsGladInitialized);

		oOpenGLContext.DisplayOpenGLContextInfo();

		glfwSetWindowCloseCallback(m_pWindow, [](GLFWwindow* pWindow)
		{
			const Window& oWindow = *reinterpret_cast<const Window*>(glfwGetWindowUserPointer(pWindow));
			events::WindowCloseEvent oWindowCloseEvent;
			
			events::SUBMIT_EVENT(oWindowCloseEvent);
		});

		glfwSetFramebufferSizeCallback(m_pWindow, [](GLFWwindow* pWindow, int nWidth, int nHeight)
		{
			glViewport(0, 0, nWidth, nHeight);
		});
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

	const GLFWwindow* Window::GetNativeWindow() const
	{
		return m_pWindow;
	}
}