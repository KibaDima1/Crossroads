#pragma once

#include <iostream>
#include <chrono>

namespace Utils {
    static int generateUniqueId() {
        static int currentId = 0;
        return ++currentId;
    }

    class Timer {
    public:
        Timer() noexcept: start_time(std::chrono::high_resolution_clock::now()) {}

        void reset() noexcept {
            start_time = std::chrono::high_resolution_clock::now();
        }

        [[nodiscard]] long long elapsed() const noexcept {
            using namespace std::chrono;
            const auto now = high_resolution_clock::now();
            return std::chrono::duration_cast<milliseconds>(now - start_time).count();
        }

    private:
        std::chrono::high_resolution_clock::time_point start_time;
    };
}