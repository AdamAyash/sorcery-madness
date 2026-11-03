#include "Application.h"

namespace engine
{
	Application::Application()
	{

	}

	Application::~Application()
	{

	}

	void Application::Run()
	{
		this->Initialize();

		while (m_bIsRunning)
		{
			Update();
		}
	}

	bool Application::Initialize()
	{
		common::Logger::Init("Client");

		WindowProperties oWindowProperties;
		m_pWindow = common::CreateScope<Window>(oWindowProperties);

		m_bIsRunning = true;

		return true;
	}

	void Application::Update()
	{
		m_pWindow->OnUpdate();
	}

	void Application::Draw()
	{

	}
}