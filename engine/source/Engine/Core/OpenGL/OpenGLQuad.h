#pragma once

#include <Common/Common.h>
#include "VertexBuffer.h"

namespace engine
{
	class OpenGLQuad final : public OpenGLObject
	{
	public:
		OpenGLQuad();
		virtual ~OpenGLQuad();

	public:
		void Generate() override;
		void Bind() const override;
		void UnBind() const override;
		void Destroy()	override;

	public:
		VertexBuffer m_oVertexBuffer;
	};
}