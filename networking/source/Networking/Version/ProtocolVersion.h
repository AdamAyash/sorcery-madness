#pragma once

#include <Common/Common.h>
#include <string>

namespace networking
{
    class ProtocolVersion : public common::NonCopyableObject
    {
    public:
        static const std::string GetVersion()
        {
            return std::to_string(MAJOR) + "." +
                std::to_string(MINOR) + "." +
                std::to_string(PATCH);
        }

    public:
        static constexpr std::uint16_t MAJOR = 0;
        static constexpr std::uint16_t MINOR = 1;
        static constexpr std::uint16_t PATCH = 0;
    };
}