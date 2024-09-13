#pragma once

#include <boost/asio/thread_pool.hpp>
#include <boost/asio/post.hpp>

#include <ranges>

#include "IRoadMediator.h"
#include "TrafficLight/TrafficLight.h"

namespace rm {
    class RoadMediator final : public IRoadMediator {
    private:
        static constexpr std::size_t kPoolSize = 3;

    private:
        std::vector<tl::TrafficLightUniquePtr> trafficLights;
        boost::asio::thread_pool pool;

    public:
        RoadMediator() noexcept: pool(kPoolSize) {}

        void addRoadTrafficLight(tl::TrafficLightUniquePtr&& trafficLight) noexcept {
            if (trafficLight == nullptr) {
                // return newError(ROAD_TRAFFIC_LIGHT_NULLPTR);
                return;
            }
            // Добавить проверку на то, что светофор уже существует
            trafficLights.push_back(std::move(trafficLight));
            // return {};
        }

        void sendEvent(tl::TrafficLightEvent event) noexcept override {
            boost::asio::post(pool, [this, event] {
                handleTrafficLightEvent(event);
            });
        }

        [[nodiscard]] const std::vector<tl::TrafficLightUniquePtr>& getTrafficLights() const noexcept override {
            return trafficLights;
        }

        void printStatu() {
            for (auto& trafficLight : trafficLights) {
                std::cout << trafficLight->getId() << std::endl;
                std::cout << "trafficLight->getQuery()" << trafficLight->getQuery() << std::endl;
                std::cout << "trafficLight->getRoadType()" << static_cast<int>(trafficLight->getRoadType()) << std::endl;
                std::cout << "trafficLight->getTrafficLightType()" << static_cast<int>(trafficLight->getTrafficLightType()) << std::endl;
            }
        }

    private:
        void handleTrafficLightEvent(tl::TrafficLightEvent event) noexcept {
            for (auto& trafficLight : trafficLights) {
                if (trafficLight->getRoadType() != event.roadwayType) {
                    BOOST_VERIFY_MSG(trafficLight->receiveEvent(event), "can't send message");
                }
            }
        }
    };
}
