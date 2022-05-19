#include "count-down-latch.h"


namespace invasion::server {
CountDownLatch::CountDownLatch(uint32_t count): m_count(count) {}

void CountDownLatch::await() {
    std::unique_lock ul{m_mutex};

    if (m_count > 0) {
        m_cv.wait(ul, [this](){ return m_count == 0; });
    }
}

void CountDownLatch::countDown() {
    std::unique_lock ul{m_mutex};
    if (m_count > 0) {
        m_count--;
        m_cv.notify_all();
    }
}

uint32_t CountDownLatch::getCount() {
    std::unique_lock ul{m_mutex};
    return m_count;
}
}