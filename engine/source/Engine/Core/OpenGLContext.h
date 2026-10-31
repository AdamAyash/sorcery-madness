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
		const bool InitializeOpenGLContext();
		const bool InitializeGlad();
		void DisplayOpenGLContextInfo();

	private:
		const inline bool CheckIfClientSupportsOpenGLVersion4() const;
	};
};
