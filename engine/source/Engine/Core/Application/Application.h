#include <Common//Common.h>
#include "Window.h"
#include "GameTime.h"
#include "../../EventSystem/Events/WindowEvents/WindowCloseEvent.h"
#include "../../EventSystem/Events/WindowEvents/WindowResizeEvent.h"
#include "../../Layers/LayerStack.h"
#include "../../EventSystem/IEventListener.h"
#include "../../Graphics/Renderer2D/Renderer2D.h"

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
		void OnEvent(IN events::Event& oEvent) override;

	protected:
		virtual bool Initialize();
		virtual void OnUpdate();
		virtual void Draw();

		bool PushLayer(ILayer* pLayer);

	private:
		bool OnWindowClose(IN const events::WindowCloseEvent& oWindowCloseEvent);
		bool OnWindowResize(IN const events::WindowResizeEvent& oWindowCloseEvent);

	private:
		bool m_bIsRunning = false;

		common::Scope<Window> m_pWindow = nullptr;
		common::Scope<LayerStack> m_pLayerStack = nullptr;
		common::Scope <Renderer2D> m_pRendered2D = nullptr;

		GameTime m_oGameTime;
	};
}