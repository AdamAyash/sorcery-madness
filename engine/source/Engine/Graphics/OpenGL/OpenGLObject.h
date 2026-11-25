#pragma once

#include <Common/Common.h>
#include <glad/glad.h>

#define INVALID_OBJECT_ID 0

namespace engine
{
	class OpenGLObject : public common::NonCopyableObject
	{
	protected:
		OpenGLObject();
		virtual ~OpenGLObject();

	public:
		unsigned int GetObjectID() const;

		virtual void Generate() PURE;
		virtual void Bind() const PURE;
		virtual void UnBind() const PURE;
		virtual void Destroy()	PURE; 

	protected:
		unsigned int m_nObjectID = INVALID_OBJECT_ID;
	};
}
