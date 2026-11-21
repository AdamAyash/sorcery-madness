#include <Common//Common.h>
#include "Window.h"
#include "../../EventSystem/IEventListener.h"
#include "../../EventSystem/Events/WindowEvents/WindowCloseEvent.h"

namespace engine
{
	class Application : public IEventListener
	{
	protected:
		Application();

	public:
		~Application();

	public:
		virtual bool Initialize();
		void Run();

	public:
		void OnEvent(const Event& oEvent) override;

	private:
		virtual void Update();
		virtual void Draw();

	private:
		bool OnWindowClose(IN const WindowCloseEvent& oWindowCloseEvent);

	private:
		bool m_bIsRunning = false;
		common::Scope<Window> m_pWindow = nullptr;
	};
}