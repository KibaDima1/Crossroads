#pragma once

#include <optional>

#include "TrafficLightType.h"

namespace tl {
    struct TrafficLightState {
    private:
        using RoadTrafficLightTypeOptional = std::optional<RoadTrafficLightType>;
        using PedestrianTrafficLightTypeOptional = std::optional<PedestrianTrafficLight>;

    public:
        RoadTrafficLightTypeOptional roadTrafficLightTypeOptional;
        PedestrianTrafficLightTypeOptional pedestrianTrafficLightTypeOptional;
    };
}