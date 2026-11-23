#include <Common//Common.h>
#include "Window.h"
#include "GameTime.h"
#include "../../EventSystem/Events/WindowEvents/WindowCloseEvent.h"
#include "../../Layers/LayerStack.h"
#include "../../EventSystem/IEventListener.h"

namespace engine
{
	class ILayer;

	class Application : public events::IEventListener
	{
	protected:
		Application();

	public:
		~Application();

	public:
		void Run();

	public:
		void OnEvent(const events::Event& oEvent) override;

	protected:
		virtual bool Initialize();
		virtual void Update();
		virtual void Draw();

		void PushLayer(ILayer* pLayer);

	private:
		bool OnWindowClose(IN const events::WindowCloseEvent& oWindowCloseEvent);

	private:
		bool m_bIsRunning = false;
		common::Scope<Window> m_pWindow = nullptr;
		common::Scope<LayerStack> m_pLayerStack = nullptr;
		GameTime m_oGameTime;
	};
}