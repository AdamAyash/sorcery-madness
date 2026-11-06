#include "GameApplication.h"
#include <Common/Logger/Logger.h>
#include "Shader.cpp"

GameApplication::GameApplication()
{
}

GameApplication::~GameApplication()
{
}

bool GameApplication::Initialize()
{
	common::Logger::Init("Client app");

	glfwInit();
	m_pWindow = glfwCreateWindow(1270, 720, "Client", nullptr, nullptr);

	glfwMakeContextCurrent(m_pWindow);

	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		LOG_ERROR("Failed to initialize glad!");
		return false;
	}

	float aspectRatio = (720.0f / 1270.0f / 2);

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

	return true;
}

void GameApplication::Run()
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

		Shader oShader("../assets/shaders/VertexShader.glsl", "../assets/shaders/FragmentShader.glsl");
		oShader.use();

		glClear(GL_COLOR_BUFFER_BIT);
		glDrawArrays(GL_TRIANGLES, 0, 6);

		glfwSwapBuffers(m_pWindow);
		glfwPollEvents();
	}
}

void GameApplication::Draw()
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