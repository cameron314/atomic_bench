#include "microbench/microbench.h"

#include <cstdio>
#include <cstdlib>
#include <atomic>

int main(int argc, char** argv)
{
	std::atomic<int> x32(0);
	int y32 = rand();
	int z32 = rand();
	if (sizeof(x32) != 4) exit(1);
	
	std::atomic<long long> x64(0);
	long long y64 = rand();
	long long z64 = rand();
	if (sizeof(x64) != 8) exit(2);
	
	printf("32-bit CAS takes %.4fns on average\n",
		moodycamel::microbench(
			[&]() { x32.compare_exchange_strong(y32, z32, std::memory_order_acq_rel); },
			1000000, /* iterations per test run */
			50 /* number of test runs */
		) * 1000 * 1000    // ms -> ns
	);
	
	printf("64-bit CAS takes %.4fns on average\n",
		moodycamel::microbench(
			[&]() { x64.compare_exchange_strong(y64, z64, std::memory_order_acq_rel); },
			1000000,
			50
		) * 1000 * 1000
	);
	
	printf("32-bit FAA takes %.4fns on average\n",
		moodycamel::microbench(
			[&]() { x32.fetch_add(y32, std::memory_order_acq_rel); },
			1000000,
			50
		) * 1000 * 1000
	);
	
	printf("64-bit FAA takes %.4fns on average\n",
		moodycamel::microbench(
			[&]() { x64.fetch_add(y64, std::memory_order_acq_rel); },
			1000000,
			50
		) * 1000 * 1000
	);
	
	return 0;
}
