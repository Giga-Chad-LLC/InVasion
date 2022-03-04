#include <atomic>
#include <iostream>
#include "physics-tick-controller.h"

namespace invasion::controllers {

PhysicsTickController::PhysicsTickController()
	: m_cancelToken(false), m_interval_ms(1000) {}

void PhysicsTickController::stop() {
	std::cout << "cancelled" << std::endl;
	m_cancelToken.store(false);
}

} // namespace invasion::controllers
