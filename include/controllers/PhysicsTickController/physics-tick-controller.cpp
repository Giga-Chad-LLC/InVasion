#include <atomic>
#include "physics-tick-controller.h"

#include <iostream>

namespace invasion::controllers {

PhysicsTickController::PhysicsTickController(const std::size_t interval)
	: m_cancelToken(false), 
	 m_isThreadActive(false),
	  m_interval_ms(interval) {}

void PhysicsTickController::stop() {
	if(m_isThreadActive.load()) {
		m_cancelToken.store(false);
		m_startedThread.join();

		m_isThreadActive.store(false);
	}
}

PhysicsTickController::~PhysicsTickController() {
	stop();
}

} // namespace invasion::controllers
