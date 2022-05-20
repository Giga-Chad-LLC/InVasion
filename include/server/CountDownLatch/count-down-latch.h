#ifndef COUNT_DOWN_LATCH_H_
#define COUNT_DOWN_LATCH_H_


#include <chrono>
#include <condition_variable>
#include <mutex>
#include <memory>
#include <iostream>

namespace invasion::server {
class CountDownLatch {
public:
    explicit CountDownLatch(uint32_t count);
    virtual ~CountDownLatch() = default;

    void await();

    template <class Rep, class Period>
    bool await(const std::chrono::duration<Rep, Period>& timeout) {
        std::unique_lock ul{m_mutex};
        bool result = true;
        if (m_count > 0) {
            result = m_cv.wait_for(ul, timeout, [this](){ return m_count == 0; });
        }
        return result;
    }

    void countDown();
    uint32_t getCount();

protected:
    std::mutex m_mutex;
    std::condition_variable m_cv;
    uint32_t m_count = 0;
};


class LatchCaller {
public:
    LatchCaller(std::shared_ptr <CountDownLatch> latch): m_latch(latch) {};
    
    ~LatchCaller() {
        this->operator()();
    }

    void operator()() {
        if (!m_isCalled) {
            m_isCalled = true;
            std::cout << "Decrease latch counter from " << m_latch->getCount();
            m_latch->countDown();
            std::cout << " to " << m_latch->getCount() << std::endl;
        }
    }

private:
    std::shared_ptr <CountDownLatch> m_latch;
    bool m_isCalled = false;
};
}

#endif