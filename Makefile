
CXX = g++ -std=c++17 -Wall

all: run_test pipes_timing

run_test: pipes_test
	./pipes_test

headers: rubrictest.hpp pipes_types.hpp pipes_algs.hpp

gnomes_test: headers pipes_test.cpp
	${CXX} pipes_test.cpp -o pipes_test

gnomes_timing: headers pipes_timing.cpp
	${CXX} pipes_timing.cpp -o pipes_timing

clean:
	rm -f pipes_test pipes_timing
