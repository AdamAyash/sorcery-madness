#pragma once

#include <glad/glad.h>
#include <GLFW/glfw3.h>

class GameApplication
{
public:
	GameApplication();
	~GameApplication();

public:
	void Run();
	bool Initialize();
	void Draw();

private:
	GLFWwindow* m_pWindow = nullptr;
};