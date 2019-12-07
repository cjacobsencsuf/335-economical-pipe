///////////////////////////////////////////////////////////////////////////////
// pipes_timing.cpp
//
// Example code showing how to run each algorithm while measuring
// elapsed times precisely. You should modify this program to gather
// all of your experimental data.
//
///////////////////////////////////////////////////////////////////////////////

#include <cassert>
#include <random>
#include <iostream>

#include "timer.hpp"

#include "pipes_algs.hpp"

void print_bar() {
  std::cout << std::string(79, '-') << std::endl;
}

int main() {

  const size_t EXHAUSTIVE_SEARCH_MAX_N = 30;

  const size_t n = 15;

  assert(n > 0);

  pipes::coordinate rows = n / 2,
                     columns = n - rows;

  unsigned cells = rows * columns,
           open_count = cells / 5,  // 20%
           rock_count = cells / 10; // 10%
  std::mt19937 gen;
  pipes::grid input = pipes::grid::random(rows, columns, open_count, 
					   rock_count, gen);

  Timer timer;
  double elapsed;

  print_bar();
  std::cout << "n=" << n
            << ", rows=" << rows
            << ", columns=" << columns
            << std::endl << std::endl;

  input.print();

  print_bar();
  std::cout << "exhaustive optimization" << std::endl;
  if (n > EXHAUSTIVE_SEARCH_MAX_N) {
    std::cout << std::endl << "(n too large, skipping exhaustive search)" 
	      << std::endl;
  } else {
    timer.reset();
    auto exhaustive_output = econ_pipes_exhaustive(input);
    elapsed = timer.elapsed();
    exhaustive_output.print();
    std::cout << std::endl << "elapsed time=" << elapsed << " seconds" 
	      << std::endl;
  }

  print_bar();
  std::cout << "dynamic programming" << std::endl;
  timer.reset();
  auto dyn_prog_output = econ_pipes_dyn_prog(input);
  elapsed = timer.elapsed();
  dyn_prog_output.print();
  std::cout << std::endl << "elapsed time=" << elapsed << " seconds" 
	    << std::endl;

  print_bar();

  return 0;
}
