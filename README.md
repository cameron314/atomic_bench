Measures the execution time of various atomic instruction primitives,
without contention.

To build and run:

    > make run

Sample output on my Intel Core 2 Duo T6500 @ 2.10 GHz:

    32-bit CAS takes 16.2929ns on average
    64-bit CAS takes 16.2876ns on average
    32-bit FAA takes 13.8286ns on average
    64-bit FAA takes 13.8282ns on average
	