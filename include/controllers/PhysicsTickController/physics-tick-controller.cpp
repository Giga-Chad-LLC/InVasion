#include <atomic>
#include "physics-tick-controller.h"

namespace invasion::controllers {

PhysicsTickController::PhysicsTickController()
	: m_cancelToken(false), m_interval_ms(8) {}

void PhysicsTickController::stop() {
	m_cancelToken.store(false);
	m_startedThread.join();
}

} // namespace invasion::controllers
