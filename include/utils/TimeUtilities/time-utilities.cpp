#include <chrono>
// utils
#include "time-utilities.h"


namespace invasion::utils {
	
long long TimeUtilities::getCurrentTime_ms() {
	return std::chrono::duration_cast<std::chrono::milliseconds>(
		std::chrono::high_resolution_clock::now().time_since_epoch()
	).count();
}

} // namespace invasion::utils
