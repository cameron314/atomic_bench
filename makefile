ifeq ($(OS),Windows_NT)
	EXT = .exe
	PLATLNOPTS =
else
	EXT =
	PLATLNOPTS = -lrt
endif

default: bench$(EXT)
run: bench$(EXT)
	./bench$(EXT)

bench.exe: bench.cpp microbench/microbench.h microbench/systemtime.h microbench/systemtime.cpp
	g++ -std=c++11 -DNDEBUG -O3 bench.cpp microbench/systemtime.cpp -o bench $(PLATLNOPTS)
