#include <thread>
#include <iostream>

#include "fixed-timeout-callback-invoker.h"


namespace invasion::controllers {
	
FixedTimeoutCallbackInvoker::~FixedTimeoutCallbackInvoker() {
	std::unique_lock l(m_mutex);

	for(std::thread& thread : m_createdThreads) {
		thread.join();
	}
}

} // namespace invasion::controllers
