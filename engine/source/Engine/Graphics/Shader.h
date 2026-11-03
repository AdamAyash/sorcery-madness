#pragma once

#include "glad/glad.h"
#include "GLFW/glfw3.h"
#include "../OpenGL/OpenGLObject.h"
#include <string>

namespace engine
{
	class Shader : public OpenGLObject
	{
	public:
		Shader(IN const std::string& strVertexShaderSourceFilePath,
			IN const std::string& strFragmentShaderSourceFilePath, const std::string& strShaderName);

		~Shader();

	public:
		virtual void Generate() override;
		virtual void Bind() const override;
		virtual void UnBind() const override;
		virtual void Destroy() override;

		void SetInteger(IN const std::string& strName, IN const int nInteger, OPTIONAL bool bBind = false);

	private:
		const bool Create(IN const std::string& strVertexShaderSourceFilePath, IN const std::string& strFragmentShaderSourceFilePath);

	private:
		std::string m_strShaderName;
	};
};
