#pragma once

#include <Common/Common.h>
#include "../../Graphics/Rendrer2D/Renderer2D.h"

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
		virtual void Draw(IN Renderer2D& oRenderer2D) PURE;
		virtual bool OnDetach() PURE;
	};
}