
#ifndef INVASIONSERVERS_QUEUE_LOGGER_H
#define INVASIONSERVERS_QUEUE_LOGGER_H

#include <thread>
#include <iostream>
#include <chrono>

struct debug {};

// pass debug{} to the queue constructor to call the debugger
class QueueLogger {
private:
    const int maxLimitItems = 40000;
    const double timeCheckSec = 1;
    const int clocksPerSec = static_cast<int>(1000000 * timeCheckSec); // 1000000 - clocks in sec
public:
    int *countItems;

    QueueLogger() : countItems(new int{0}) {}

    void start() {
        std::thread([&]() {
            while (true) {
                if ((clock()) / clocksPerSec % 1 == 0 &&
                    clock() % clocksPerSec == 1) { // second condition need to print exactly once
                    std::cout << "time(sec): " << clock() / CLOCKS_PER_SEC;
                    std::cout << " | size of queue now: " << " " << *countItems << '\n';
                }
                if (*countItems > maxLimitItems) {
                    std::cout << "count items in queue exceeded the limit!\n";
                }
            }
        }).detach();
    }

    ~QueueLogger() {
        delete countItems;
    }
};

#endif //INVASIONSERVERS_QUEUE_LOGGER_H
