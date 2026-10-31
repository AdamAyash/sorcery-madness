#include "OpenGLContext.h"
#include "glad/glad.h"
#include "GLFW/glfw3.h"
#include <Common/Assert.h>

namespace engine
{
	OpenGLContext::OpenGLContext()
	{
	}

	OpenGLContext::~OpenGLContext()
	{
	}

	const bool OpenGLContext::InitializeOpenGLContext()
	{
		const int nIsGLFWInitialized = glfwInit();
		ASSERT(nIsGLFWInitialized);

		glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
		glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 4);
		glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

		return true;
	}

	const bool OpenGLContext::InitializeGlad()
	{
		if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
		{
			LOG_ERROR("Failed to initialize glad!");
			return false;
		}

		ASSERT(CheckIfClientSupportsOpenGLVersion4());

		return true;
	}

	void OpenGLContext::DisplayOpenGLContextInfo()
	{
		LOG_INFO("OpenGL Vendor: {0}", reinterpret_cast<const char*>(glGetString(GL_VENDOR)));
		LOG_INFO("OpenGL Renderer:{0}", reinterpret_cast<const char*>(glGetString(GL_RENDERER)));
		LOG_INFO("OpenGL Version: {0}", reinterpret_cast<const char*>(glGetString(GL_VERSION)));
		LOG_INFO("OpenGL Shading Language Version: {0}", reinterpret_cast<const char*>(glGetString(GL_SHADING_LANGUAGE_VERSION)));
	}

	const bool OpenGLContext::CheckIfClientSupportsOpenGLVersion4() const
	{
		return GLVersion.major > 4 || (GLVersion.major == 4 && GLVersion.minor >= 3);
	}
}
