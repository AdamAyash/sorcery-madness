#pragma once

#include <Common/Common.h>
#include "../OpenGL/OpenGLQuad.h"
#include "../OpenGL/Shader.h"
#include "../../GameObjects/Transform.h"

namespace engine
{
	class Application;

	struct RenderCommand
	{
		RenderCommand()
		{
		}

		Transform oTransform;
		Shader* pShader = nullptr;
	};

	class Renderer2D final : public common::NonCopyableObject
	{

	private:
		Renderer2D();

	public:
		~Renderer2D();

	public:
		void Submit(IN const RenderCommand& oRenderCommand);

	private:
		void Draw();

	private:
		OpenGLQuad m_oOpenGLQuad;
		glm::mat4 m_oProjectionMatrix = glm::mat4(1);
		std::vector<RenderCommand> m_oRenderCommands;

		friend Application;
	};
}