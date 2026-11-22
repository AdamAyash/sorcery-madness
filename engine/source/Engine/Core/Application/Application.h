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
		void Run();

	public:
		void OnEvent(const Event& oEvent) override;

	protected:
		virtual bool Initialize();
		virtual void Update();
		virtual void Load();
		virtual void Draw();

		void PushLayer(ILayer* pLayer);
		void PushLayerOverlay(ILayer* pLayer);

	private:
		bool OnWindowClose(IN const WindowCloseEvent& oWindowCloseEvent);

	private:
		bool m_bIsRunning = false;
		common::Scope<Window> m_pWindow = nullptr;
	};
}