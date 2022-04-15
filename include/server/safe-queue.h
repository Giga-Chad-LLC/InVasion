#ifndef SAFE_QUEUE
#define SAFE_QUEUE

#include <mutex>
#include <condition_variable>

#include <queue>
#include <utility>
#include "queue-logger.h"

template<class T>
class SafeQueue {
    QueueLogger Logger;
    std::queue<T> q;
    int64_t inf = 1;
    std::mutex mtx;
    std::condition_variable cv;

    std::condition_variable syncWait;
    bool finishProcessing = false;
    int syncCounter = 0;

    void decreaseSyncCounter() {
        if (--syncCounter == 0) {
            syncWait.notify_one();
        }
    }

public:

    typedef typename std::queue<T>::size_type sizeType;

    SafeQueue() {}

    SafeQueue(debug) {
        Logger.start();
    }

    ~SafeQueue() {
        finish();
    }

    void produce(T &&item) {
        std::lock_guard<std::mutex> lock(mtx);
        *Logger.cntItems++;
        q.push(std::move(item));
        cv.notify_one();

    }

    sizeType size() {

        std::lock_guard<std::mutex> lock(mtx);

        return q.size();

    }

    [[nodiscard]]
    bool consume(T &item) {

        std::lock_guard<std::mutex> lock(mtx);

        if (q.empty()) {
            return false;
        }

        item = std::move(q.front());
        q.pop();
        *Logger.cntItems--;
        return true;

    }

    [[nodiscard]]
    bool consumeSync(T &item) {

        std::unique_lock<std::mutex> lock(mtx);

        syncCounter++;

        cv.wait(lock, [&] {
            return !q.empty() || finishProcessing;
        });

        if (q.empty()) {
            decreaseSyncCounter();
            return false;
        }
        *Logger.cntItems--;
        item = std::move(q.front());
        q.pop();

        decreaseSyncCounter();
        return true;

    }

    void finish() {

        std::unique_lock<std::mutex> lock(mtx);

        finishProcessing = true;
        cv.notify_all();

        syncWait.wait(lock, [&]() {
            return syncCounter == 0;
        });

        finishProcessing = false;

    }

};

#endif // SAFE_QUEUE