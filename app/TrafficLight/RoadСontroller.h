#pragma once

#include <vector>

#include "ITrafficLight.h"

namespace rd {
    class RoadController {
    private:
        std::vector<tl::RoadTrafficLightUniqPtr> roadTrafficLights;
        std::vector<tl::PedestrianTrafficLight> pedestrianTrafficLights;

    public:
        RoadController();

        ~RoadController();


    };
}