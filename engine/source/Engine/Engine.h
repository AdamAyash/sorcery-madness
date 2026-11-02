#pragma once

#include "GLFW/glfw3.h"

namespace engine
{
	class Engine
	{
	public:
		static Engine& GetInstance();

	private:
		Engine() = default;
		Engine(const Engine&) = delete;
		Engine(Engine&&) = delete;
		Engine& operator=(const Engine&) = delete;
		Engine& operator=(Engine&&) = delete;

	public:
		bool Initialize();
		void Run();

	private:
		GLFWwindow* m_pWindow = nullptr;
	};
}