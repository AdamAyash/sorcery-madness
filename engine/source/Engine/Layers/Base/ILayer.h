#pragma once

#include <Common/Common.h>

namespace engine
{
	class ILayer : public common::NonCopyableObject
	{
	protected:
		ILayer()
		{
		}
	
	public:
		virtual ~ILayer()
		{
		}

	public:
		virtual bool OnAttach() PURE;
		virtual void Update() PURE;
		virtual void Draw();
		virtual bool OnDettach() PURE;
	};
}