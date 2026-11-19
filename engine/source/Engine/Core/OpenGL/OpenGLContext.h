#pragma once

namespace engine
{
	class OpenGLContext
	{
	public:
		OpenGLContext();
		~OpenGLContext();

	public:
		const bool InitializeOpenGLContext();
		const bool InitializeGlad();
		void DisplayOpenGLContextInfo();

	private:
		const inline bool CheckIfClientSupportsOpenGLVersion4() const;
	};
};
