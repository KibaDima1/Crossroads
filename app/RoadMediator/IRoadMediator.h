#pragma once

#include "TrafficLight/ITrafficLight.h"

#include "ErrorCode/ErrorCode.h"
#include "TrafficLight/TrafficLightEvent.h"

namespace rm {
    struct IRoadMediator {
    public:
        virtual ~IRoadMediator() = default;

        virtual ErrorCode::Status sendEvent(tl::TrafficLightEvent event) noexcept = 0;

        virtual tl::TrafficLightState requestTrafficLightState(tl::Id id) noexcept = 0;
    };

    using RoadMediatorSharedPtr = std::shared_ptr<IRoadMediator>;
}