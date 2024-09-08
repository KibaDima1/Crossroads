#pragma once

#include <cstdint>

namespace tl
{
    enum class RoadTrafficLightType : uint8_t {
        RED,
        ORANGE,
        GREEN
    };

    enum class PedestrianTrafficLight : uint8_t {
        RED,
        GREEN
    };
}