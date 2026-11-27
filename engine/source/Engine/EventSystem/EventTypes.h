#pragma once

namespace engine::events
{
	enum class EventTypes
	{
		WindowCloseEvent,
		WindowResizeEvent
	};

	inline const std::string GetEventTypeName(const EventTypes eEventType)
	{
		switch (eEventType)
		{
		case EventTypes::WindowCloseEvent:
			return "WindowCloseEvent";
		case EventTypes::WindowResizeEvent:
			return "WindowResizeEvent";
		default: 
			return "Not implemented event";
		}
	}
}