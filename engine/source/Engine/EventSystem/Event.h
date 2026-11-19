#pragma once

#include <Common/Common.h>
#include "EventTypes.h"

namespace engine
{
    class Event : public common::NonCopyableObject
    {
    protected:
        Event() = default;
        virtual ~Event() = default;

    public:
        virtual EventTypes GetEventType() const PURE;
    };

#define EVENT(type) \
    static EventTypes GetStaticEventType() { return EventTypes::type; } \
    EventTypes GetEventType() const override { return GetStaticEventType(); }
}