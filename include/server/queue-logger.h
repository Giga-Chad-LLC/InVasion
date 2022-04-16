
#ifndef INVASIONSERVERS_QUEUE_LOGGER_H
#define INVASIONSERVERS_QUEUE_LOGGER_H

#include <thread>
#include <iostream>
#include <chrono>

struct debug {};

// pass {} to the queue constructor to call the debugger
class QueueLogger {
private:
    const int maxLimitItems = 40000;
    const double time_check_sec = 1;
    const int clocks_per_sec = static_cast<int>(1000000 * time_check_sec); // 1000000 - clocks in sec
public:
    int *cntItems;

    QueueLogger() : cntItems(new int{0}) {}

    void start() {
        std::thread([&]() {
            while (true) {
                if ((clock()) / clocks_per_sec % 1 == 0 &&
                    clock() % clocks_per_sec == 1) { // second condition need to print exactly once
                    std::cout << "time(sec): " << clock() / CLOCKS_PER_SEC;
                    std::cout << " | size of queue now: " << " " << *cntItems << '\n';
                }
                if (*cntItems > maxLimitItems) {
                    std::cout << "count items in queue exceeded the limit!\n";
                }
            }
        }).detach();
    }

    ~QueueLogger() {
        delete cntItems;
    }
};

#endif //INVASIONSERVERS_QUEUE_LOGGER_H
