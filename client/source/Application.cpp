#include "Application.h"
#include <Common/Logger/Logger.h>
#include <gtc/matrix_transform.hpp>

float velocity = 200;

glm::vec3 m_oPlayerPosition = glm::vec3(300, 300, 0.5);
glm::vec3 m_oPlayerRotation;
glm::vec3 m_oPlayerScale = glm::vec3( 16.0f, 16.0f, 1.0);

double deltaTime = 0;
double lastTime = 0;
double currentTime = 0;

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	if (key == GLFW_KEY_A && (action == GLFW_PRESS || action == GL_REPEAT))
	{
		m_oPlayerPosition.x -= velocity * deltaTime;
	}

	if (key == GLFW_KEY_D && (action == GLFW_PRESS || action == GL_REPEAT))
	{
		m_oPlayerPosition.x += velocity * deltaTime;
	}

	if (key == GLFW_KEY_W && (action == GLFW_PRESS || action == GL_REPEAT))
	{
		m_oPlayerPosition.y += velocity * deltaTime;
	}

	if (key == GLFW_KEY_S && (action == GLFW_PRESS || action == GL_REPEAT))
	{
		m_oPlayerPosition.y -= velocity * deltaTime;
	}
}

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
	m_pWindow = glfwCreateWindow(1280, 720, "Client", nullptr, nullptr);

	glfwMakeContextCurrent(m_pWindow);

	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		LOG_ERROR("Failed to initialize glad!");
		return false;
	}

	glfwSetKeyCallback(m_pWindow, key_callback);

	glViewport(0, 0, 1280, 720);

	float vertices[] = {
		-0.5f , -0.5f, 0.0f,
		 0.5f , -0.5f, 0.0f,
		 0.5f ,  0.5f, 0.0f,
		-0.5f , -0.5f, 0.0f,
		 0.5f ,  0.5f, 0.0f,
		-0.5f ,  0.5f, 0.0f
	};

	GLuint VBO, VAO;
	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);

	glGenBuffers(1, &VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	m_oShader = std::make_unique<Shader>("assets/shaders/VertexShader.glsl", "assets/shaders/FragmentShader.glsl");

	glClearColor(
		0.063f,
		0.094f,
		0.153f,
		1.0f
	);

	return true;
}

void Application::Update()
{
	lastTime = currentTime;
	currentTime = glfwGetTime();
	deltaTime = currentTime - lastTime;

	m_oModelMatrix = glm::mat4(1.f);
	m_oModelMatrix = glm::translate(m_oModelMatrix, m_oPlayerPosition);
	m_oModelMatrix = glm::scale(m_oModelMatrix, m_oPlayerScale);

	m_oProjectionMatrix = glm::ortho(0.0f, 1280.0f, 0.0f, 720.0f, -1.0f, 1.0f);
}

void Application::Run()
{
	Initialize();

	while (!glfwWindowShouldClose(m_pWindow))
	{
		glfwPollEvents();

		Update();
		Draw();

		glfwSwapBuffers(m_pWindow);
	}
}
void Application::Draw()
{
	m_oShader->use();
	m_oShader->setMatrix("model", m_oModelMatrix);
	m_oShader->setMatrix("projection", m_oProjectionMatrix);

	glClear(GL_COLOR_BUFFER_BIT);

	glDrawArrays(GL_TRIANGLES, 0, 6);
}