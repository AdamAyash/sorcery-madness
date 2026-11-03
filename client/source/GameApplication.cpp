#include "GameApplication.h"

GameApplication::GameApplication()
{
}

GameApplication::~GameApplication()
{
}

bool GameApplication::Initialize()
{
	if (!BASE::Initialize())
	{
		return false;
	}

	float vertices[] =
	{
		1
	};

	GLuint VBO, VAO;
	glGenBuffers(1, &VAO);
	glBindVertexArray(VAO);

	glGenBuffers(1, &VBO);
	glBindBuffer(GL_VERTEX_ARRAY, VBO);
	glBufferData(GL_VERTEX_ARRAY, sizeof(vertices), vertices, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, FALSE, 3 * sizeof(float), (void*)0);

	engine::Shader oShader("assets/VertexShader.vert", "assets/FragmentShader.frag", "");
	m_oShader = oShader;
}

void GameApplication::Draw()
{
	glClear(GL_COLOR);

	m_oShader.Bind();

	glDrawArrays(GL_TRIANGLES, 0, 3);
}