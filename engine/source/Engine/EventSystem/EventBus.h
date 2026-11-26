#pragma once

#include <Common/Common.h>
#include <map>

namespace engine::events
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
		static EventBus &GetInstance();

		void Register(IN IEventListener *pEventListener);
		void UnRegister(IN IEventListener *pEventListener);

		void Sumbit(IN Event &oEvent);

	private:
		std::map<unsigned long, IEventListener *> m_oEventListenersMap;
	};
}