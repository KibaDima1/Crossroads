#pragma once

#include <queue>
#include <thread>
#include <boost/thread/thread.hpp>
#include <condition_variable>

#include "TrafficLightState/GreenTrafficLightState.h"
#include "TrafficLightState/RedTrafficLightState.h"
#include "TrafficLightState/OrangeTrafficLightState.h"
#include "RoadDetector/IRoadDetector.h"

#include "RoadMediator/IRoadMediator.h"

#include "RoadwayType.h"
#include "ITrafficLight.h"

namespace tl {
    class TrafficLight : public ITrafficLight {
    private:
        static constexpr std::size_t kMaxEventQueueSize = 1000;
        static constexpr std::uint16_t kResetTimeSeconds = 1;

    private:
        std::thread th;

        std::condition_variable cv;
        Utils::Timer timer;
        std::mutex mtx;

        Id id;

        std::queue<TrafficLightEvent> eventQueue;
        rm::RoadMediatorSharedPtr roadMediatorPtr;
        state::ITrafficLightStateUniqPtr trafficLightStatePtr;
        rd::RoadDetectorPtr roadDetectorPtr;
        RoadwayType roadwayType;

        TrafficLightType lastTrafficLightType;

    public:
        TrafficLight(
                rd::RoadDetectorPtr&& roadDetectorPtr,
                RoadwayType roadwayTypeNew,
                rm::RoadMediatorSharedPtr roadMediatorSharedPtr,
                state::ITrafficLightStateUniqPtr&& trafficLightUniqPtr,
                TrafficLightType newLastTrafficLightType) noexcept:
                roadDetectorPtr(std::move(roadDetectorPtr)),
                roadwayType(roadwayTypeNew),
                roadMediatorPtr(roadMediatorSharedPtr),
                trafficLightStatePtr(std::move(trafficLightUniqPtr)),
                lastTrafficLightType(newLastTrafficLightType),
                th(&TrafficLight::run, this){
            std::cout << "trafficLightStatePtr->getState()" << static_cast<int>(trafficLightStatePtr->getState()) << std::endl;
            timer.reset();
            id = Utils::IdGenerator::getInstance().generate();
        }

        ErrorCode::Status sendEvent(const TrafficLightEvent& trafficLightEvent) noexcept override {
            roadMediatorPtr->sendEvent(trafficLightEvent);
            return {};
        }

        ErrorCode::Status receiveEvent(const TrafficLightEvent& trafficLightEvent) noexcept override {
            sleep(1);
            std::lock_guard lock(mtx);
            if (eventQueue.size() >= kMaxEventQueueSize) {
                return newError(TRAFFIC_LIGHT_QUEUE_OVERFLOW);
            }
            eventQueue.push(trafficLightEvent);
            return{};
        }

        TrafficLightType getTrafficLightType() noexcept override {
            std::lock_guard lock(mtx);
            return trafficLightStatePtr->getState();
        }

        RoadwayType getRoadType() noexcept override {
            return roadwayType;
        }

        const Id& getId() noexcept override {
            return id;
        }

        std::size_t getQuery() noexcept override {
            return roadDetectorPtr->getDetectedObjects();
        }

    public:
        void run() {
            while(true) {
                std::variant<TrafficLightEvent, std::monostate> eventVariant = eventPop();
                if (std::holds_alternative<TrafficLightEvent>(eventVariant)) {
                    auto& event = std::get<TrafficLightEvent>(eventVariant);
                    if (trafficLightStatePtr->isStateChangeNeeded(
                            event,
                            roadDetectorPtr->getDetectedObjects(),
                            roadwayType)) {
                        updateTrafficLightState();
                    }
                }

                if (trafficLightStatePtr->getState() == TrafficLightType::GREEN) {
                    const TrafficLightEvent event{roadwayType, roadDetectorPtr->getDetectedObjects(), false};
                    BOOST_VERIFY_MSG(sendEvent(event), "can't send event");
                }

                timer.reset();
            }
        }

        std::variant<TrafficLightEvent, std::monostate> eventPop() noexcept {
            std::lock_guard lock(mtx);
            if (!eventQueue.empty()) {
                const TrafficLightEvent event = eventQueue.front();
                eventQueue.pop();
                return event;
            }
            return {};
        }

        void updateTrafficLightState() noexcept {
            if ((trafficLightStatePtr->getState() == TrafficLightType::ORANGE) &&
                    (roadwayType != RoadwayType::PEDESTRIAN)) {
                if (lastTrafficLightType == TrafficLightType::RED) {
                    std::lock_guard lock(mtx);
                    trafficLightStatePtr = std::make_unique<state::GreenTrafficLightState>();
                    std::queue<TrafficLightEvent> emptyQueue;
                    eventQueue.swap(emptyQueue);
                    lastTrafficLightType = TrafficLightType::GREEN;
                    roadDetectorPtr->setState(TrafficLightType::RED);
                } else {
                    std::lock_guard lock(mtx);
                    trafficLightStatePtr = std::make_unique<state::RedTrafficLightState>();
                    std::queue<TrafficLightEvent> emptyQueue;
                    eventQueue.swap(emptyQueue);
                    lastTrafficLightType = TrafficLightType::RED;
                    roadDetectorPtr->setState(TrafficLightType::RED);
                }
            } else if (trafficLightStatePtr->getState() == TrafficLightType::RED) {
                const std::vector<TrafficLightUniquePtr>& trafficLights = roadMediatorPtr->getTrafficLights();
                bool needUpdateState = false;
                while ((needUpdateState == false) || (timer.elapsed() >= 20)) {
                    for (const auto& trafficLight : trafficLights) {
                        if (trafficLight->getRoadType() != roadwayType &&
                                    trafficLight->getTrafficLightType()  > TrafficLightType::RED) {
                            break;
                        }
                        needUpdateState = true;
                        std::lock_guard lock(mtx);
                        trafficLightStatePtr = std::make_unique<state::OrangeTrafficLightState>();
                        std::queue<TrafficLightEvent> emptyQueue;
                        eventQueue.swap(emptyQueue);
                        roadDetectorPtr->setState(TrafficLightType::ORANGE);
                    }
                }
            } else {
                std::lock_guard lock(mtx);
                trafficLightStatePtr = std::make_unique<state::OrangeTrafficLightState>();
                std::queue<TrafficLightEvent> emptyQueue;
                eventQueue.swap(emptyQueue);
                roadDetectorPtr->setState(TrafficLightType::ORANGE);
            }
        }
    };
}