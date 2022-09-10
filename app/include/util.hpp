#pragma once

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
