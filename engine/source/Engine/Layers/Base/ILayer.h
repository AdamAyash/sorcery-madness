#pragma once

#include <Common/Common.h>
#include "../../Graphics/Renderer2D/Renderer2D.h"
#include "../../EventSystem/IEventListener.h"

namespace engine
{
	class ILayer : public events::IEventListener
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
		virtual void OnUpdate() PURE;
		virtual void Draw(IN Renderer2D& oRenderer2D) PURE;
		virtual bool OnDetach() PURE;
	};
}