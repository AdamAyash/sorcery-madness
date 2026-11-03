#pragma once 

#include "glad/glad.h"
#include "GLFW/glfw3.h"
#include <Common/Common.h>
#include <string>

#define DEFAULT_WINDOW_WIDTH 1600
#define DEFAULT_WINDOW_HEIGHT 900

#define DEFAULT_TITLE "Application"

namespace engine
{
	struct WindowProperties
	{
		WindowProperties(const std::string& strTitle = DEFAULT_TITLE
			, int nWidth = DEFAULT_WINDOW_WIDTH, int nHeight = DEFAULT_WINDOW_HEIGHT)
			: m_strTitle(strTitle), m_nWidth(nWidth), m_nHeight(nHeight)
		{
		}

		~WindowProperties()
		{
		}

		int m_nWidth;
		int m_nHeight;

		std::string m_strTitle;
	};

	class Window final
	{
	public:
		Window(IN const WindowProperties& oWindowProperties);
		~Window();

	public:
		void OnUpdate();
		constexpr GLFWwindow* GetNativeWindow() const;

	private:
		void Initialize(IN const WindowProperties& oWindowProperties);
		void ShutDown();

	private:
		GLFWwindow* m_pWindow;
	};
};
