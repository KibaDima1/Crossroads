#pragma once

#include <queue>
#include <thread>
#include <boost/thread/thread.hpp>
#include <condition_variable>

#include "RoadMediator/IRoadMediator.h"

#include "RoadwayType.h"
#include "ITrafficLight.h"

namespace tl {
    class TrafficLightBase : public ITrafficLight {
    private:
        static constexpr std::size_t kMaxEventQueueSize = 1000;

    private:
        boost::thread th;
        std::mutex mtx;
        std::condition_variable cv;
        std::queue<TrafficLightEvent> eventQueue;
        rm::RoadMediatorSharedPtr roadMediatorPtr;
        RoadwayType _roadwayType;
        QueueSize _queueSize = 0;
        Id _id;

    public:
        TrafficLightBase(
                QueueSize queueSize,
                RoadwayType roadwayType,
                Id id,
                rm::RoadMediatorSharedPtr roadMediatorSharedPtr) noexcept:
                _queueSize(queueSize),
                _roadwayType(roadwayType),
                _id(id),
                roadMediatorPtr(roadMediatorSharedPtr),
                th(&TrafficLightBase::run, this){}

        Id getId() noexcept override {
            return _id;
        }

        ErrorCode::Status sendEvent(const TrafficLightEvent& trafficLightEvent) noexcept override {
            return roadMediatorPtr->sendEvent(trafficLightEvent);
        }

        ErrorCode::Status receiveEvent(const TrafficLightEvent& trafficLightEvent) noexcept override {
            std::lock_guard lock(mtx);
            if (eventQueue.size() >= kMaxEventQueueSize) {
                return newError(TRAFFIC_LIGHT_QUEUE_OVERFLOW);
            }
            eventQueue.push(trafficLightEvent);
            cv.notify_one();
            return{};
        }

        void run() noexcept override {
            std::unique_lock lock(mtx);
            cv.wait(lock, [&]() noexcept { return !eventQueue.empty(); });
            const TrafficLightEvent event = eventQueue.front();
            eventQueue.pop();
            lock.unlock();
            // логика работы светофора
        }

        // TrafficLightStateRc queryTrafficLightState(Id id) noexcept override {
        //
        // } функция для наследника
    };
}