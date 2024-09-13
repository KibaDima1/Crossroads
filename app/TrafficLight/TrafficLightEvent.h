#pragma once

#include <cstdint>
#include <cstddef>

#include "RoadwayType.h"

namespace tl {
    using Id = std::uint16_t;
    using QueueSize = std::size_t;

    struct TrafficLightEvent {
    public:
        RoadwayType roadwayType;
        QueueSize queueSize;
        bool needToSwitch;
    };
}