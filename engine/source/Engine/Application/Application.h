#include "Window.h"
#include <Common/Common.h>

namespace engine
{
    class Application
    {
	protected:
		Application();
		virtual ~Application();

	public:
		void Run();
		virtual bool Initialize();
		virtual void Update();
		virtual void Draw();

	private:
		bool m_bIsRunning = false;
		common::Scope<Window> m_pWindow = nullptr;
    };
}