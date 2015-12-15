#include "microbench/microbench.h"

#include <cstdio>
#include <cstdlib>
#include <atomic>
#include <thread>
#include <cstdint>

#ifdef _WIN32
#define WIN32_LEAN_AND_MEAN
#define NOMINMAX
#include <windows.h>
#define THREAD_LOCAL __declspec(thread)
#else
#define THREAD_LOCAL thread_local
#endif

uintptr_t ghetto_thread_id()
{
	static THREAD_LOCAL int x;
	return reinterpret_cast<uintptr_t>(&x);
}

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
	
	printf("std::this_thread::get_id() takes %.4fns on average\n",
		moodycamel::microbench(
			[&]() { std::this_thread::get_id(); },
			10000,
			50
		) * 1000 * 1000
	);
	
#ifdef _WIN32
	printf("GetCurrentThreadId() takes %.4fns on average\n",
		moodycamel::microbench(
			[&]() { GetCurrentThreadId(); },
			1000000,
			50
		) * 1000 * 1000
	);
#endif
	
#if !defined(__APPLE__) || (!defined(TARGET_IPHONE_SIMULATOR) && !defined(TARGET_OS_IPHONE))
	volatile uintptr_t ghettoId = 0;
	printf("ghetto_thread_id() takes %.4fns on average\n",
		moodycamel::microbench(
		[&]() { ghettoId += ghetto_thread_id(); },
			1000000,
			50
		) * 1000 * 1000
	);
#endif
	
	return 0;
}
