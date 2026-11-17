#pragma once

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "Client/GameClient.h"
#include "Shader.cpp"
#include <memory>
#include <mat4x4.hpp>

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods);

class Application
{
public:
	Application();
	~Application();

public:
	void Run();
	bool Initialize();
	void Update();
	void Draw();
private:
	GLFWwindow* m_pWindow = nullptr;
	GameClient m_oGameClient;

	std::unique_ptr<Shader> m_oShader = nullptr;

	glm::mat4 m_oModelMatrix = glm::mat4(1.f);
	glm::mat4 m_oProjectionMatrix = glm::mat4(1.f);
};
