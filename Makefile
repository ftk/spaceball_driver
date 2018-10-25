.PHONY: all
all: driver driver_dbg driver_kbm

driver: $(wildcard *.h *.cpp)
	g++ $(wildcard *.cpp) -L. -lvjoyinterface -Ofast -march=native -DNDEBUG -fwhole-program -o $@

driver_dbg: $(wildcard *.h *.cpp)
	g++ $(wildcard *.cpp) -L. -lvjoyinterface -g -Og -DDEBUG -Wall -o $@

driver_kbm: $(wildcard *.h *.cpp)
	g++ $(wildcard *.cpp) -DNO_VJOY -O3 -march=native -DNDEBUG -o $@
