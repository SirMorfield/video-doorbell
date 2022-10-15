#pragma once
#include <chrono>
#include <vector>

#define ASSERT(left, operator, right)                                                                                               \
	{                                                                                                                               \
		if (!((left) operator(right))) {                                                                                            \
			std::cerr << "ASSERTION FAILED: " << #left << " " << #operator<< " " << #right << " function " << __func__ << ", file " \
					  << "./" << __FILE__ << ":" << __LINE__ << "\n"                                                                \
					  << #left << " = " <<(left)                                                                                    \
					  << "\n"                                                                                                       \
					  << #right << " = " << (right) << std::endl;                                                                   \
			exit(EXIT_FAILURE);                                                                                                     \
		}                                                                                                                           \
	}
std::vector<std::string> ft_split(const std::string& s, const std::string& charset);

class Timeout {
  public:
	Timeout(std::chrono::seconds::rep duration) : _duration(duration) {
		this->update();
	}

	void update() {
		this->_start = std::chrono::high_resolution_clock::now();
	}

	bool expired() const {
		const auto call_duration_sec = std::chrono::duration_cast<std::chrono::seconds>(std::chrono::high_resolution_clock::now() - this->_start).count();
		return call_duration_sec >= this->_duration;
	}

  private:
	std::chrono::time_point<std::chrono::high_resolution_clock> _start;
	std::chrono::seconds::rep									_duration;
};
