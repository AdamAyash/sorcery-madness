#include "Application.h"
#include "../Core/OpenGLContext.h"

namespace engine
{
	Application& Application::GetInstance()
	{
		static Application oApplicationInstance;
		return oApplicationInstance;
	}

	bool Application::Start()
	{
		OpenGLContext oOpenGLContext;
		oOpenGLContext.InitializeOpenGLContext();

		return true;
	}
}