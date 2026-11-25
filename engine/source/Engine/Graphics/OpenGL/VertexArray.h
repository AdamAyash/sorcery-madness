#pragma once

#include <vector>
#include "OpenGLObject.h"
#include "VertexBuffer.h"

namespace engine
{
	class VertexArray : public OpenGLObject
	{
	public:
		VertexArray();
		virtual ~VertexArray();

	public:
		virtual void Generate() override;
		virtual void Bind() const override;
		virtual void UnBind() const override;
		virtual void Destroy() override;

		void AddVertexBuffer(common::Reference<VertexBuffer> pVertrexBuffer);

	private:
		std::vector<common::Reference<VertexBuffer>> m_oVertexBufferArray;
	};
}
