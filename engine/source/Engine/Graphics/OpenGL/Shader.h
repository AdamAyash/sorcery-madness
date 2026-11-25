#pragma once

#include "OpenGLObject.h"
#include <string>
#include <GLFW/glfw3.h>
#include <matrix.hpp>

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

		bool IsShaderValid() const;

		void SetMatrix(IN const std::string& strName, IN const glm::mat4& oMatrix);

	private:
		const bool Create(IN const std::string& strVertexShaderSourceFilePath, IN const std::string& strFragmentShaderSourceFilePath);

	private:
		const std::string m_strShaderName;
		bool m_bIsShaderValid = false;
	};
};
