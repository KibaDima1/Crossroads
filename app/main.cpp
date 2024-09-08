
#include <iostream>
#include <unistd.h>

#include "TrafficLight/ITrafficLight.h"
#include "RoadMediator/RoadMediator.h"


#include "ErrorCode/ErrorCode.h"

bool isRunning = true;

void processInput() {
    // Симулируем нажатие клавиши выхода
    char input;
    std::cin >> input;
    if (input == 'q') {
        isRunning = false;
    }
}

int main() {
    while (isRunning) {
        processInput();
    }
    return 0;
}