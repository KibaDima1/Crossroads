#include <atomic>

#include "TrafficLight/TrafficLightFactory.h"

std::atomic<bool> isRunning{true};

void processInput() {
    char input;
    std::cin >> input;
    if (input == 'q') {
        isRunning = false;
    }
}

int main() {
    rm::RoadMediator roadMediator;
    rm::RoadMediatorSharedPtr roadMediatorPtr(&roadMediator);

    roadMediator.addRoadTrafficLight(tl::TrafficLightFactory::make(tl::RoadwayType::TWO, roadMediatorPtr));
    roadMediator.addRoadTrafficLight(tl::TrafficLightFactory::make(tl::RoadwayType::TWO, roadMediatorPtr));

    roadMediator.addRoadTrafficLight(tl::TrafficLightFactory::make(tl::RoadwayType::PEDESTRIAN, roadMediatorPtr));
    roadMediator.addRoadTrafficLight(tl::TrafficLightFactory::make(tl::RoadwayType::PEDESTRIAN, roadMediatorPtr));
    roadMediator.addRoadTrafficLight(tl::TrafficLightFactory::make(tl::RoadwayType::PEDESTRIAN, roadMediatorPtr));
    roadMediator.addRoadTrafficLight(tl::TrafficLightFactory::make(tl::RoadwayType::PEDESTRIAN, roadMediatorPtr));

    roadMediator.addRoadTrafficLight(tl::TrafficLightFactory::make(tl::RoadwayType::ONE, roadMediatorPtr));
    roadMediator.addRoadTrafficLight(tl::TrafficLightFactory::make(tl::RoadwayType::ONE, roadMediatorPtr));

    while (isRunning) {
        processInput();
    }
    return 0;
}