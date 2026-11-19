#pragma once

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "Client/Client.h"
#include "Shader.cpp"
#include <memory>
#include <mat4x4.hpp>

class GameContext;

class Game
{
public:
	Game();
	~Game();

public:
	void Run();
	bool Initialize();
	void Update();
	void Draw();

private:
	void ProcessInput();

private:
	GLFWwindow* m_pWindow = nullptr;

	common::Scope<GameContext> m_oGameContext = nullptr;
	common::Scope<Client> m_oClient = nullptr;

	std::unique_ptr<Shader> m_oShader = nullptr;

	glm::mat4 m_oModelMatrix = glm::mat4(1.f);
	glm::mat4 m_oProjectionMatrix = glm::mat4(1.f);
};
