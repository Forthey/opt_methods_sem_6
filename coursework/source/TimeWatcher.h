#pragma once

#include <chrono>


class TimeWatcher {
    std::chrono::time_point<std::chrono::steady_clock> startPoint;
    std::chrono::time_point<std::chrono::steady_clock> stopPoint;
public:
    void start() {
        startPoint = std::chrono::steady_clock::now();
    }

    void stop() {
        stopPoint = std::chrono::steady_clock::now();
    }

    double getLastElapsedSeconds() {
        std::chrono::duration<double> elapsed_seconds = stopPoint - startPoint;

        return elapsed_seconds.count();
    }
};
