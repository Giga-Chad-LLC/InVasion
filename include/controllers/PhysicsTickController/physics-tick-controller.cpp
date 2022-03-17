#include <atomic>
#include "physics-tick-controller.h"

namespace invasion::controllers {

PhysicsTickController::PhysicsTickController(const std::size_t interval)
	: m_cancelToken(false), m_interval_ms(interval) {}

void PhysicsTickController::stop() {
	m_cancelToken.store(false);
	m_startedThread.join();
}

PhysicsTickController::~PhysicsTickController() {
	// calling stop() to finish created thread
	stop();
}

} // namespace invasion::controllers
