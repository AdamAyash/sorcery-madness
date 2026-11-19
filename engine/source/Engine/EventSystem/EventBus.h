#pragma once

#include <Common/Common.h>

namespace engine
{
	class Event;
	class IEventListener;

	class EventBus final : public common::NonCopyableObject
	{
	private:
		EventBus();
	public:
		~EventBus();

	public:
		static EventBus& GetInstance();

		void Subscibe(IN IEventListener* pEventListener);
		void Sumbit(IN const Event& oEvent);

	private:
		std::vector<IEventListener*> m_oEventListeners;
	};

	#define SUBMIT_EVENT(Event) EventBus::GetInstance().Sumbit(Event);
	#define SUBSCRIBE() EventBus::GetInstance().Subscibe(this);
}