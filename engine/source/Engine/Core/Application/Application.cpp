#include "Application.h"
#include "../../Layers/Base/ILayer.h"
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
		m_pLayerStack = common::CreateScope<LayerStack>();

		events::REGISTER_EVENT_LISTENER(this);

		m_bIsRunning = true;

		return true;
	}

	void Application::Run()
	{
		if (!Initialize())
		{
			LOG_ERROR("Failed to initilize the engine.");
			return;
		}

		while (m_bIsRunning)
		{
			Update();
			Draw();

			m_pWindow->OnUpdate();
		}
	}

	void Application::OnEvent(IN const events::Event &oEvent)
	{
		events::EventDispatcher oEventDispatcher(oEvent);
		oEventDispatcher.Dispatch<events::WindowCloseEvent>(BIND_EVENT(Application::OnWindowClose));
	}

	void Application::Update()
	{
		m_oGameTime.Calculate();

		m_pLayerStack->Update();
	}

	void Application::Draw()
	{
		m_pLayerStack->Draw();
	}

	void Application::PushLayer(ILayer* pLayer)
	{
		m_pLayerStack->PushLayer(pLayer);
	}

	bool Application::OnWindowClose(IN const events::WindowCloseEvent &oWindowCloseEvent)
	{
		m_bIsRunning = false;
		return true;
	}
}