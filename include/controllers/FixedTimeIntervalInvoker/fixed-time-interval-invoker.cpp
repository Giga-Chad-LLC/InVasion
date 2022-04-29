#include <atomic>
#include "fixed-time-interval-invoker.h"

#include <iostream>

namespace invasion::controllers {

FixedTimeIntervalInvoker::FixedTimeIntervalInvoker(const std::size_t interval)
	: m_cancelToken(false), 
	 m_isThreadActive(false),
	  m_interval_ms(interval) {}

void FixedTimeIntervalInvoker::stop() {
	if(m_isThreadActive.load()) {
		m_cancelToken.store(false);
		m_startedThread.join();

		m_isThreadActive.store(false);
	}
}

FixedTimeIntervalInvoker::~FixedTimeIntervalInvoker() {
	stop();
}

} // namespace invasion::controllers
