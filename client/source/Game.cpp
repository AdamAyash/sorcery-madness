#include "Game.h"
#include <Common/Logger/Logger.h>
#include <gtc/matrix_transform.hpp>
#include "Client/GameContext/GameContext.h"

float velocity = 200;

glm::vec3 m_oPlayerRotation;
glm::vec3 m_oPlayerScale = glm::vec3( 16.0f, 16.0f, 1.0);

double deltaTime = 0;
double lastTime = 0;
double currentTime = 0;

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
}

Game::Game()
{
}

Game::~Game()
{
}

bool Game::Initialize()
{
	common::Logger::Init("Client");

	m_oGameContext = common::CreateScope<GameContext>();
	m_oClient = common::CreateScope<Client>("127.0.0.1", "3000", *m_oGameContext);

	if (!m_oClient->Start())
	{
		//TODO LOG
		return false;
	}

	if (!m_oClient->ConnectToServer())
	{
		//TODO LOG
		return false;
	}

	while (!m_oGameContext->IsClientStatusConnected() && !m_oGameContext->GetSessionId())
	{
		m_oClient->Update();
	}

	glfwInit();
	m_pWindow = glfwCreateWindow(1280, 720, "Client", nullptr, nullptr);

	glfwMakeContextCurrent(m_pWindow);

	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		LOG_ERROR("Failed to initialize glad!");
		return false;
	}

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

void Game::Update()
{
	m_oClient->Update();

	lastTime = currentTime;
	currentTime = glfwGetTime();
	deltaTime = currentTime - lastTime;

	m_oProjectionMatrix = glm::ortho(0.0f, 1280.0f, 0.0f, 720.0f, -1.0f, 1.0f);
}

void Game::Run()
{
	Initialize();

	while (!glfwWindowShouldClose(m_pWindow))
	{
		glfwPollEvents();

		ProcessInput();
		Update();
		Draw();

		glfwSwapBuffers(m_pWindow);
	}
}
void Game::Draw()
{
	glClear(GL_COLOR_BUFFER_BIT);

	for (auto playerIterator = m_oGameContext->oPlayersMap.begin(); playerIterator != m_oGameContext->oPlayersMap.end(); playerIterator++)
	{
		m_oModelMatrix = glm::mat4(1.f);
		m_oModelMatrix = glm::translate(m_oModelMatrix, playerIterator->second);
		m_oModelMatrix = glm::scale(m_oModelMatrix, m_oPlayerScale);

		m_oShader->use();
		m_oShader->setMatrix("model", m_oModelMatrix);
		m_oShader->setMatrix("projection", m_oProjectionMatrix);

		glDrawArrays(GL_TRIANGLES, 0, 6);
	}
}

void Game::ProcessInput()
{
	if (glfwGetKey(m_pWindow, GLFW_KEY_A) == GLFW_PRESS)
	{
		//m_oGameContext->oPlayerPosition.x -= velocity * deltaTime;
	}

	if (glfwGetKey(m_pWindow, GLFW_KEY_D) == GLFW_PRESS)
	{
		//m_oGameContext->oPlayerPosition.x += velocity * deltaTime;
	}

	if (glfwGetKey(m_pWindow, GLFW_KEY_W) == GLFW_PRESS)
	{
		//m_oGameContext->oPlayerPosition.y += velocity * deltaTime;
	}

	if (glfwGetKey(m_pWindow, GLFW_KEY_S) == GLFW_PRESS)
	{
		//m_oGameContext->oPlayerPosition.y -= velocity * deltaTime;
	}
}