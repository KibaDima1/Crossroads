#pragma once

#include "TrafficLight/ITrafficLight.h"

#include "TrafficLight/TrafficLightEvent.h"

namespace rm {
    struct IRoadMediator {
    public:
        virtual ~IRoadMediator() = default;

        virtual void sendEvent(tl::TrafficLightEvent event) noexcept = 0;

        [[nodiscard]] virtual const std::vector<tl::TrafficLightUniquePtr>& getTrafficLights() const noexcept = 0;
    };

    using RoadMediatorSharedPtr = std::shared_ptr<IRoadMediator>;
}