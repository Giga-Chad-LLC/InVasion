#ifndef FIXED_TIMEOUT_CALLBACK_INVOKER_H_
#define FIXED_TIMEOUT_CALLBACK_INVOKER_H_

#include <thread>
#include <functional>
#include <chrono>
#include <vector>


namespace invasion::controllers {

class FixedTimeoutCallbackInvoker {
public:
	template <class Func, class... Args>
	void setTimeout(std::size_t timeout_ms, Func&& f, Args&&... args) {
		auto callback = std::bind(std::forward<Func>(f), std::forward<Args>(args)...);
		auto thread = std::move(std::thread([callback, timeout_ms]() mutable {
			std::this_thread::sleep_for(std::chrono::milliseconds(timeout_ms));
			callback();
		}));

		m_createdThreads.push_back(std::move(thread));
	}

	~FixedTimeoutCallbackInvoker();

private:
	std::vector<std::thread> m_createdThreads;
};

} // namespace invasion::controllers


#endif // FIXED_TIMEOUT_CALLBACK_INVOKER_H_
