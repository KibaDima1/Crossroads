#pragma once

#include <iostream>
#include <atomic>

#include <iostream>
#include <chrono>

namespace Utils {
    class IdGenerator {
    public:
        // Возвращаем единственный экземпляр IdGenerator
        static IdGenerator& getInstance() {
            static IdGenerator instance; // Единственный экземпляр
            return instance;
        }

        // Генерация уникального ID
        unsigned long long generate() {
            return counter.fetch_add(1, std::memory_order_relaxed);
        }

        // Удаляем методы копирования и перемещения, чтобы избежать создания копий
        IdGenerator(const IdGenerator&) = delete;
        IdGenerator& operator=(const IdGenerator&) = delete;
        IdGenerator(IdGenerator&&) = delete;
        IdGenerator& operator=(IdGenerator&&) = delete;

    private:
        // Приватный конструктор
        IdGenerator() : counter(0) {}

        std::atomic<unsigned long long> counter;
    };

    class Timer {
    public:
        Timer() noexcept : start_time(std::chrono::high_resolution_clock::now()), is_stopped(false) {}

        void reset() noexcept {
            start_time = std::chrono::high_resolution_clock::now();
            is_stopped = false;
        }

        void stop() noexcept {
            if (!is_stopped) {
                stop_time = std::chrono::high_resolution_clock::now();
                is_stopped = true;
            }
        }

        [[nodiscard]] long long elapsed() const noexcept {
            using namespace std::chrono;
            const auto end_time = is_stopped ? stop_time : high_resolution_clock::now();
            return std::chrono::duration_cast<seconds>(end_time - start_time).count();
        }

    private:
        std::chrono::high_resolution_clock::time_point start_time;
        std::chrono::high_resolution_clock::time_point stop_time;
        bool is_stopped;
    };
}