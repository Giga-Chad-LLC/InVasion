#include <thread>
#include <iostream>

#include "fixed-timeout-callback-invoker.h"


namespace invasion::controllers {
	
FixedTimeoutCallbackInvoker::~FixedTimeoutCallbackInvoker() {
	for(std::thread& thread : m_createdThreads) {
		thread.join();
	}
}

} // namespace invasion::controllers
