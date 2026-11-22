#pragma once

namespace engine::events
{
	enum class EventTypes
	{
		WindowCloseEvent
	};

	inline const std::string GetEventTypeName(const EventTypes eEventType)
	{
		switch (eEventType)
		{
		case EventTypes::WindowCloseEvent:
			return "WindowCloseEvent";
		default: 
			return "Not implemented event";
		}
	}
}