#ifndef FIXED_TIMEOUT_CALLBACK_INVOKER_H_
#define FIXED_TIMEOUT_CALLBACK_INVOKER_H_

#include <thread>
#include <functional>
#include <chrono>
#include <vector>
#include <mutex>


namespace invasion::controllers {
	
class FixedTimeoutCallbackInvoker {
public:
	template <class Func, class... Args>
	void setTimeout(std::size_t timeout_ms, Func&& f, Args&&... args) {
		auto callback = std::bind(std::forward<Func>(f), std::forward<Args>(args)...);

		std::unique_lock l(m_mutex);

		m_createdThreads.emplace_back([callback, timeout_ms]() {
			std::this_thread::sleep_for(std::chrono::milliseconds(timeout_ms));
			callback();
		});
	}

	~FixedTimeoutCallbackInvoker();

private:
	mutable std::mutex m_mutex;
	std::vector<std::thread> m_createdThreads;
};

} // namespace invasion::controllers


#endif // FIXED_TIMEOUT_CALLBACK_INVOKER_H_
