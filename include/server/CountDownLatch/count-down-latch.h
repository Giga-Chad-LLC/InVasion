#ifndef COUNT_DOWN_LATCH_H_
#define COUNT_DOWN_LATCH_H_


#include <chrono>
#include <condition_variable>
#include <mutex>

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


template <class Func>
class Caller {
public:
    Caller(Func func): m_func(func), m_isCalled(false) {}
    
    ~Caller() {
        if (m_isCalled) {
            m_isCalled = true;
            std::cout << "Call the func()" << std::endl;
            m_func();
        }
    }

    void call() {
        if (!m_isCalled) {
            m_isCalled = true;
            std::cout << "Call the func()" << std::endl;
            m_func();
        }
    }
private:
    Func m_func;
    bool m_isCalled;
};
}

#endif