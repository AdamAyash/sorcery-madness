#include "Application.h"
#include "../../EventSystem/EventDispatcher.h"
#include "../../EventSystem/EventBus.h"

namespace engine
{
	Application::Application()
	{
	}

	Application::~Application()
	{
	}

	bool Application::Initialize()
	{
		common::Logger::Init("Application");

		WindowProperties oWindowProperties;
		m_pWindow = common::CreateScope<Window>(oWindowProperties);

		REGISTER_EVENT_LISTENER(this);

		m_bIsRunning = true;

		return true;
	}

	void Application::Run()
	{
		while (m_bIsRunning)
		{
			Update();
			Draw();
		}
	}

	void Application::OnEvent(IN const Event &oEvent)
	{
		EventDispatcher oEventDispatcher(oEvent);
		oEventDispatcher.Dispatch<WindowCloseEvent>(BIND_EVENT(Application::OnWindowClose));
	}

	void Application::Update()
	{
	}

	void Application::Draw()
	{
		m_pWindow->OnUpdate();
	}

	bool Application::OnWindowClose(IN const WindowCloseEvent &oWindowCloseEvent)
	{
		m_bIsRunning = false;
		return true;
	}
}