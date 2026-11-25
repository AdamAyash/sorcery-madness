#pragma once

#include ""
#include "string"
#include "GLFW/glfw3.h"

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

	private:
		const bool Create(IN const std::string& strVertexShaderSourceFilePath, IN const std::string& strFragmentShaderSourceFilePath);

	private:
		std::string m_strShaderName;
	};
};
