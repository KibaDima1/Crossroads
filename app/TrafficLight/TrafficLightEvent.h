#pragma once

#include <cstdint>
#include <cstddef>

#include "TrafficLightType.h"

namespace tl {
    using Id = std::uint16_t;
    using QueueSize = std::size_t;

    struct TrafficLightEvent {
    private:
        using RoadTrafficLightTypeOptional = std::optional<RoadTrafficLightType>;
        using PedestrianTrafficLightTypeOptional = std::optional<PedestrianTrafficLight>;

    public:
        RoadTrafficLightTypeOptional roadTrafficLightTypeOptional;
        PedestrianTrafficLightTypeOptional pedestrianTrafficLightTypeOptional;
        QueueSize queueSize;
        Id senderId;
        Id receiverId;
    };
}