#pragma once

#include "OpenGLObject.h"
#include <vector>

#define TWO_COMPONENT_SIZE 2
#define FOUR_COMPONENT_SIZE 4

namespace engine
{
	class VertexBuffer : public OpenGLObject
	{
		class VertexBufferLayout : public OpenGLObject
		{
		public:
			VertexBufferLayout(unsigned int nIndex, std::size_t nnStrideSize, int nComponentSize = FOUR_COMPONENT_SIZE,
				GLenum eComponentType = GL_FLOAT, GLboolean bNormalized = GL_FALSE, const void* pCurrentStideOffset = (void*)0);

			~VertexBufferLayout();

		public:
			virtual void Generate() override;
			virtual void Bind() const override;
			virtual void UnBind() const override;
			virtual void Destroy() override;

		private:
			unsigned int m_nIndex;
			int m_nComponentSize;
			GLenum m_eComponentType;
			GLboolean m_bNormalized;
			std::size_t m_nnStrideSize;
			const void* m_pCurrentStideOffset;
		};

	public:
		VertexBuffer();
		~VertexBuffer();

	public:
		virtual void Generate() override; 
		virtual void Bind() const override;
		virtual void UnBind() const override;
		virtual void Destroy() override;

		void AddLayout(IN const std::size_t nnStrideSize, OPTIONAL const  int nComponentSize = FOUR_COMPONENT_SIZE, OPTIONAL const GLenum eComponentType = GL_FLOAT,
			OPTIONAL const GLboolean bNormalized = GL_FALSE, const void* pCurrentStideOffset = (void*)0);

		template<class DataType>
		void Pack(IN std::vector<DataType>& pData, IN const GLenum usage)
		{
			const auto nSize = sizeof(pData) / sizeof(DataType);
			glBufferData(GL_ARRAY_BUFFER, nSize, pData.data(), usage);
		}

	private:
		std::vector<common::Reference<VertexBufferLayout>> m_oVertexBufferLayoutArray;
		unsigned int m_nBufferLayoutCounter;
	};
}

