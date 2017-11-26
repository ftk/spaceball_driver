driver: $(wildcard *.h *.cpp)
	g++ $(wildcard *.cpp) -L. -lvjoyinterface -O3 -march=native -DNDEBUG -o $@

driver_dbg: $(wildcard *.h *.cpp)
	g++ $(wildcard *.cpp) -L. -lvjoyinterface -g -DDEBUG -o $@

driver_kbm: $(wildcard *.h *.cpp)
	g++ $(wildcard *.cpp) -DNO_VJOY -O3 -march=native -DNDEBUG -o $@
