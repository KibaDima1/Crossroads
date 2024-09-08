#pragma once

#include <boost/asio/thread_pool.hpp>
#include <boost/asio/post.hpp>

#include <ranges>

#include "IRoadMediator.h"
#include "TrafficLight/TrafficLightBase.h"


namespace rm {
    class RoadMediator final : public IRoadMediator {
    private:
        static constexpr std::size_t kPoolSize = 10;

    private:
        boost::asio::thread_pool pool;
        std::vector<tl::TrafficLightSharedPtr> trafficLights;

    public:
        RoadMediator() noexcept: pool(kPoolSize) {}

        ErrorCode::Status addRoadTrafficLight(tl::TrafficLightSharedPtr trafficLight) noexcept {
            if (trafficLight == nullptr) {
                return newError(ROAD_TRAFFIC_LIGHT_NULLPTR);
            }
            // Добавить проверку на то, что светофор уже существует
            trafficLights.emplace_back(trafficLight);
            return {};
        }

        ErrorCode::Status sendEvent(tl::TrafficLightEvent event) noexcept override {
            boost::asio::post(pool, [this, event] {
                handleTrafficLightEvent(event);
            });
            return {};
        }

        tl::TrafficLightState requestTrafficLightState(tl::Id id) noexcept override {
            const auto& it = std::ranges::find_if(
                    trafficLights,
                    [&id](const tl::TrafficLightSharedPtr& trafficLight) {
                return trafficLight->getId() == id;
            });

            if (it == trafficLights.end()) {
                // LOG
                return {};
            }
            if (it->get()->getId() == id) {
                // LOG
                return {};
            }

            return it->get()->getTrafficLightState();
        }

    private:
        void handleTrafficLightEvent(tl::TrafficLightEvent event) noexcept {
            const auto& it = std::ranges::find_if(
                    trafficLights,
                    [&event](const tl::TrafficLightSharedPtr& trafficLight) {
                return trafficLight->getId() == event.receiverId;
            });

            if (it == trafficLights.end()) {
                // LOG
                return;
            }
            if (it->get()->getId() == event.senderId) {
                // LOG
                return;
            }

            BOOST_VERIFY_MSG(it->get()->receiveEvent(event), "can't send message");
        }
    };
}
