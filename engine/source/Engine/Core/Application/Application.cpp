#pragma once

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
		UNREGISTER_EVENT_LISTENER(this);
	}

	bool Application::Initialize()
	{
		common::Logger::Init("Application");

		WindowProperties oWindowProperties;
		m_pWindow = common::CreateScope<Window>(oWindowProperties);
		m_pLayerStack = common::CreateScope<LayerStack>();
		m_pRendered2D = common::CreateScopeFromRaw<Renderer2D>(new Renderer2D());

		REGISTER_EVENT_LISTENER(this);

		m_bIsRunning = true;

		return true;
	}

	void Application::Run()
	{
		if (!Initialize())
		{
			LOG_ERROR("Failed to initialize the engine.");
			return;
		}

		while (m_bIsRunning)
		{
			OnUpdate();
			Draw();

			m_pWindow->OnUpdate();
		}
	}

	void Application::OnEvent(IN events::Event &oEvent)
	{
		events::EventDispatcher oEventDispatcher(oEvent);
		oEventDispatcher.Dispatch<events::WindowCloseEvent>(BIND_EVENT(Application::OnWindowClose));
		oEventDispatcher.Dispatch<events::WindowResizeEvent>(BIND_EVENT(Application::OnWindowResize));

		m_pLayerStack->OnEvent(oEvent);
	}

	void Application::OnUpdate()
	{
		m_oGameTime.Calculate();

		m_pLayerStack->OnUpdate();
	}

	void Application::Draw()
	{
		m_pLayerStack->Draw(*m_pRendered2D.get());
		m_pRendered2D->Draw();
	}

	bool Application::PushLayer(ILayer *pLayer)
	{
		return m_pLayerStack->PushLayer(pLayer);
	}

	bool Application::OnWindowClose(IN const events::WindowCloseEvent &oWindowCloseEvent)
	{
		m_bIsRunning = false;
		return true;
	}

	bool Application::OnWindowResize(IN const events::WindowResizeEvent& oWindowCloseEvent)
	{
		m_pRendered2D->SetWindowViewport(oWindowCloseEvent.GetWidth(), oWindowCloseEvent.GetHeight());
		return true;
	}
}