#pragma once

#include <memory>

#include "TrafficLightState.h"

#include "TrafficLightEvent.h"
#include "ErrorCode/ErrorCode.h"

namespace tl {
    struct ITrafficLight {
    public:
        using TrafficLightStateRc = ErrorCode::CustomError<TrafficLightState>;

    public:
        virtual ~ITrafficLight() = default;

        virtual Id getId() noexcept = 0;

        virtual ErrorCode::Status sendEvent(const TrafficLightEvent& trafficLightEvent) noexcept = 0;

        virtual ErrorCode::Status receiveEvent(const TrafficLightEvent& trafficLightEvent) noexcept = 0;

        // virtual ErrorCode::Status sendEventOnTimer(std::uint64_t intervalInSeconds, Id id) noexcept = 0;

        virtual TrafficLightStateRc queryTrafficLightState(Id id) = 0;

        virtual TrafficLightState getTrafficLightState() noexcept = 0;

        virtual ErrorCode::Status popEvent() noexcept = 0;

        virtual void run() noexcept = 0;
    };

    using TrafficLightSharedPtr = std::shared_ptr<ITrafficLight>;
}
