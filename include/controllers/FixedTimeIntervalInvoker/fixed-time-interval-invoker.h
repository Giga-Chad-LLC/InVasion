#ifndef FIXED_TIME_INTERVAL_INVOKER_H_
#define FIXED_TIME_INTERVAL_INVOKER_H_

#include <atomic>
#include <chrono>
#include <thread>
#include <cassert>
#include <utility>
#include <functional>

namespace invasion::controllers {

// instance of FixedTimeIntervalInvoker is created for every session
class FixedTimeIntervalInvoker {
public:
	explicit FixedTimeIntervalInvoker(std::size_t interval_ms);

	template <class Func, class... Args>
	void start(Func&& f, Args&&... args) {
		// calling start without finishing previous call is not allowed
		assert(!m_cancelToken.load());

		m_isThreadActive.store(true);
		m_cancelToken.store(true);
		auto callback = std::bind(std::forward<Func>(f), std::forward<Args>(args)...);

		m_startedThread = std::move(std::thread(
			[callback, this]() mutable {
				while (m_cancelToken.load()) {
					callback();
					std::this_thread::sleep_for(std::chrono::milliseconds(m_interval_ms));
				}
			}
		));
	}

	void stop();
	~FixedTimeIntervalInvoker();

private:
	std::atomic_bool m_cancelToken;
	std::atomic_bool m_isThreadActive;
	const std::size_t m_interval_ms;
	std::thread m_startedThread;
};

} // namespace invasion::controllers



#endif // FIXED_TIME_INTERVAL_INVOKER_H_
