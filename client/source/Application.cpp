#include "Application.h"
#include <Common/Logger/Logger.h>
#include <filesystem>
#include "Shader.cpp"

Application::Application()
	: m_oGameClient("127.0.0.1", "3000")
{
}

Application::~Application()
{
}

bool Application::Initialize()
{
	common::Logger::Init("Client");

	if (!m_oGameClient.Start())
	{
		//TODO LOG
		return false;
	}

	if (!m_oGameClient.ConnectToServer())
	{
		//TODO LOG
		return false;
	}

	while (m_oGameClient.GetClientConnectionStatus() != ClientConnectionStatuses::Connected)
	{
		m_oGameClient.Update();
	}

	glfwInit();
	m_pWindow = glfwCreateWindow(1270, 720, "Client", nullptr, nullptr);

	glfwMakeContextCurrent(m_pWindow);

	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		LOG_ERROR("Failed to initialize glad!");
		return false;
	}

	float aspectRatio = (720.0f / 1270.0f);

	float vertices[] = {
		-0.5f * aspectRatio, -0.5f, 0.0f,
		 0.5f * aspectRatio, -0.5f, 0.0f,
		 0.5f * aspectRatio,  0.5f, 0.0f,

		-0.5f * aspectRatio, -0.5f, 0.0f,
		 0.5f * aspectRatio,  0.5f, 0.0f,
		-0.5f * aspectRatio,  0.5f, 0.0f
	};

	GLuint VBO, VAO;
	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);

	glGenBuffers(1, &VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	Shader oShader("..//assets//shaders//VertexShader.glsl", "..//assets//shaders//FragmentShader.glsl");
	oShader.use();

	return true;
}

void Application::Run()
{
	Initialize();

	while (!glfwWindowShouldClose(m_pWindow))
	{
		glClearColor(
			0.063f,
			0.094f,
			0.153f,
			1.0f
		);

		Draw();

		glfwSwapBuffers(m_pWindow);
		glfwPollEvents();
	}
}
void Application::Draw()
{
	glClear(GL_COLOR_BUFFER_BIT);

	glClearColor(
		0.063f,
		0.094f,
		0.153f,
		1.0f
	);

	glDrawArrays(GL_TRIANGLES, 0, 6);
}