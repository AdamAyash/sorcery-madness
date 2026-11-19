#pragma once

#include "Event.h"
#include <functional>

namespace engine
{
	template <typename TEvent>
	using EventCallback = std::function<bool(const TEvent&)>;

	class EventDispatcher : public common::NonCopyableObject
	{
	public:
		EventDispatcher(IN const Event& oEvent)
			: m_oEvent(oEvent)
		{
		}

		~EventDispatcher()
		{
		}

	public:
		template <class TEvent>
		bool Dispatch(IN EventCallback<TEvent> oEventCallback)
		{
			if (m_oEvent.GetEventType() == TEvent::GetStaticEventType())
			{
				oEventCallback(dynamic_cast<const TEvent&>(m_oEvent));
			}

			return true;
		}

	private:
		const Event& m_oEvent;
	};
}

#define BIND_EVENT(oFucntionReference) std::bind(&oFucntionReference, this, std::placeholders::_1)
