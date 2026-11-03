#include "Shader.h"
#include <Common/Assert.h>
#include <Common/Timer.h>
#include "../FileSystem/FileSystem.h"


#define SHADER_COMPILATION_INFO_BUFFER_SIZE 512

namespace engine
{
	Shader::Shader(IN const std::string& strVertexShaderSourceFilePath,
		IN const std::string& strFragmentShaderSourceFilePath, const std::string& strShaderName)
		: m_strShaderName(strShaderName)
	{
		common::Timer oTimer;
		if (!Create(strVertexShaderSourceFilePath, strFragmentShaderSourceFilePath))
		{
			LOG_ERROR("Shader creation failed.");
		}

		LOG_WARN("Shader ({0}) creation took: {1} ms.", m_strShaderName, oTimer.ElapsedMilliseconds());
	}

	Shader::~Shader()
	{
		Destroy();
	}

	void Shader::Generate()
	{
	}

	void Shader::Bind() const
	{
		glUseProgram(m_nObjectID);
	}

	void Shader::UnBind() const
	{
	}

	void Shader::Destroy()
	{
		glDeleteProgram(m_nObjectID);
	}

	void Shader::SetInteger(IN const std::string& strName, IN const int nInteger, OPTIONAL bool bBind /*= false*/)
	{
		if (bBind)
			Bind();
		glUniform1i(glGetUniformLocation(this->m_nObjectID, strName.c_str()), nInteger);
	}

	const bool Shader::Create(IN const std::string& strVertexShaderSourceFilePath, IN const std::string& strFragmentShaderSourceFilePath)
	{
		const std::string strVertexShaderSource = FileSystem::ReadFileContents(strVertexShaderSourceFilePath);
		const char* const pVertexShaderSource = strVertexShaderSource.c_str();

		if (!pVertexShaderSource)
		{
			ASSERT(pVertexShaderSource);
			return false;
		}

		const std::string strFragmentShaderSource = FileSystem::ReadFileContents(strFragmentShaderSourceFilePath);
		const char* const pFragmentShaderSource = strFragmentShaderSource.c_str();

		if (!pFragmentShaderSource)
		{
			ASSERT(pFragmentShaderSource);
			return false;
		}

		BOOL bIsSuccessful = FALSE;
		char shaderCompilationInfoBuffer[SHADER_COMPILATION_INFO_BUFFER_SIZE];

		const GLuint nVertexShaderProgram = glCreateShader(GL_VERTEX_SHADER);
		glShaderSource(nVertexShaderProgram, 1, &pVertexShaderSource, NULL);
		glCompileShader(nVertexShaderProgram);

		glGetShaderiv(nVertexShaderProgram, GL_COMPILE_STATUS, &bIsSuccessful);
		if (!bIsSuccessful)
		{
			glGetShaderInfoLog(nVertexShaderProgram, SHADER_COMPILATION_INFO_BUFFER_SIZE, NULL, shaderCompilationInfoBuffer);
			LOG_ERROR(shaderCompilationInfoBuffer);
			return false;
		}

		const GLuint nFragmentShaderProgram = glCreateShader(GL_FRAGMENT_SHADER);
		glShaderSource(nFragmentShaderProgram, 1, &pFragmentShaderSource, NULL);
		glCompileShader(nFragmentShaderProgram);

		glGetShaderiv(nFragmentShaderProgram, GL_COMPILE_STATUS, &bIsSuccessful);
		if (!bIsSuccessful)
		{
			glGetShaderInfoLog(nFragmentShaderProgram, SHADER_COMPILATION_INFO_BUFFER_SIZE, NULL, shaderCompilationInfoBuffer);
			LOG_ERROR(shaderCompilationInfoBuffer);
			return false;
		}

		m_nObjectID = glCreateProgram();
		glAttachShader(m_nObjectID, nVertexShaderProgram);
		glAttachShader(m_nObjectID, nFragmentShaderProgram);
		glLinkProgram(m_nObjectID);

		glGetProgramiv(m_nObjectID, GL_LINK_STATUS, &bIsSuccessful);
		if (!bIsSuccessful)
		{
			glGetProgramInfoLog(m_nObjectID, SHADER_COMPILATION_INFO_BUFFER_SIZE, NULL, shaderCompilationInfoBuffer);
			LOG_ERROR(shaderCompilationInfoBuffer);
			return false;
		}

		glDeleteShader(nVertexShaderProgram);
		glDeleteShader(nFragmentShaderProgram);

		return true;
	}
};
