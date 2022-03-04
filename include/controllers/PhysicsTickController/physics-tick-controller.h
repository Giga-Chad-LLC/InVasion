#ifndef PHYSICS_TICK_CONTROLLER_H_
#define PHYSICS_TICK_CONTROLLER_H_

#include <atomic>
#include <chrono>
#include <thread>
#include <future>
#include <iostream>

namespace invasion::controllers {
	
	
// instance of PhysicsTickController is created for every session
struct PhysicsTickController {
	explicit PhysicsTickController();

	template <class Func, class... Args>
	void start(Func&& f, Args&&... args) {
		m_cancelToken.store(true);
		auto callback = std::bind(std::forward<Func>(f), std::forward<Args>(args)...);
		
		std::cout << "outer address: " << &m_cancelToken << std::endl;

		static_cast<void>(
			std::async(std::launch::async, [callback, this]() mutable {
				while (m_cancelToken.load()) {
					callback();
					std::cout << "inner address: " << &m_cancelToken << std::endl;
					std::this_thread::sleep_for(std::chrono::milliseconds(m_interval_ms));
				}
			})
		);
	}

	void stop();

private:
	std::atomic_bool m_cancelToken;
	const std::size_t m_interval_ms;
};

} // namespace invasion::controllers



#endif // PHYSICS_TICK_CONTROLLER_H_