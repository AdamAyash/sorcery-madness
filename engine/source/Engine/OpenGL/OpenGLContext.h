#pragma once

#include <Common/Common.h>

namespace engine
{
	class OpenGLContext final : public common::NonCopyableObject
	{
	public:
		OpenGLContext();
		~OpenGLContext();

	public:
		bool InitializeOpenGLContext() const;
		bool InitializeGlad() const;
		void DisplayOpenGLContextInfo();
	private:
		inline bool CheckIfClientSupportsOpenGLVersion4() const;
	};
};
