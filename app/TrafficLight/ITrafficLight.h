#pragma once

#include "TrafficLightType.h"

#include "TrafficLightEvent.h"
#include "ErrorCode/ErrorCode.h"

namespace tl {
    struct ITrafficLight {
    public:
        virtual ~ITrafficLight() = default;

        virtual ErrorCode::Status sendEvent(const TrafficLightEvent& trafficLightEvent) noexcept = 0;

        virtual ErrorCode::Status receiveEvent(const TrafficLightEvent& trafficLightEvent) noexcept = 0;

        virtual TrafficLightType getTrafficLightType() noexcept = 0;

        virtual RoadwayType getRoadType() noexcept = 0;

        virtual const Id& getId() noexcept = 0;

        virtual std::size_t getQuery() noexcept = 0;

        // virtual void start() noexcept = 0;
        //
        // virtual void stop() noexcept = 0;
    };

    using TrafficLightUniquePtr = std::unique_ptr<ITrafficLight>;
}
