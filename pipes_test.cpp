///////////////////////////////////////////////////////////////////////////////
// pipes_test.cpp
//
// Unit tests for pipes_algs.hpp
//
///////////////////////////////////////////////////////////////////////////////

#include <cassert>
#include <random>

#include "rubrictest.hpp"

#include "pipes_types.hpp"
#include "pipes_algs.hpp"

int main() {

  Rubric rubric;

  const pipes::step_direction R = pipes::STEP_DIRECTION_RIGHT,
                              D = pipes::STEP_DIRECTION_DOWN;

  pipes::grid empty2(2, 2), empty4(4, 4);
  pipes::path empty2_solution(empty2), empty4_solution(empty4);

  pipes::grid horizontal(4, 4);
  horizontal.set(0, 3, pipes::CELL_OPEN);
  pipes::path horizontal_solution(horizontal, {R, R, R});

  pipes::grid vertical(4, 4);
  vertical.set(3, 0, pipes::CELL_OPEN);
  pipes::path vertical_solution(vertical, {D, D, D});

  pipes::grid all_open(4, 4);
  for (pipes::coordinate r = 0; r < 4; ++r) {
    for (pipes::coordinate c = 0; c < 4; ++c) {
      if (!(r == 0 && c == 0)) {
        all_open.set(r, c, pipes::CELL_OPEN);
      }
    }
  }

  pipes::grid maze(4, 4);
  /*    ..XX
        X..X
        XX..
        XXXO   */
  maze.set(0, 2, pipes::CELL_ROCK);
  maze.set(0, 3, pipes::CELL_ROCK);
  maze.set(1, 0, pipes::CELL_ROCK);
  maze.set(1, 3, pipes::CELL_ROCK);
  maze.set(2, 0, pipes::CELL_ROCK);
  maze.set(2, 1, pipes::CELL_ROCK);
  maze.set(3, 0, pipes::CELL_ROCK);
  maze.set(3, 1, pipes::CELL_ROCK);
  maze.set(3, 2, pipes::CELL_ROCK);
  maze.set(3, 3, pipes::CELL_OPEN);

  pipes::path maze_solution(maze, {R, D, R, D, R, D});

  std::mt19937 gen;
  pipes::grid small_random =  pipes::grid::random(4, 5, 6, 4, gen),
    medium_random = pipes::grid::random(12, 24, 20, 20, gen),
    large_random =  pipes::grid::random(20, 80, 30, 70, gen);

  rubric.criterion("exhaustive search - simple cases", 4,
		   [&]() {
         TEST_EQUAL("empty2", empty2_solution, econ_pipes_exhaustive(empty2));
         TEST_EQUAL("empty4", empty4_solution, econ_pipes_exhaustive(empty4));
         TEST_EQUAL("horizontal", horizontal_solution, 
		    econ_pipes_exhaustive(horizontal));
         TEST_EQUAL("vertical", vertical_solution, 
		    econ_pipes_exhaustive(vertical));
         auto output = econ_pipes_exhaustive(all_open);
         TEST_EQUAL("all_open path length", 7, output.steps().size());
         TEST_EQUAL("all_open total open cells", 6, output.total_open());
		   });

  rubric.criterion("exhaustive search - maze", 1,
		   [&]() {
         TEST_EQUAL("correct", maze_solution, econ_pipes_exhaustive(maze));
		   });

  rubric.criterion("dynamic programming - simple cases", 4,
		   [&]() {
         TEST_EQUAL("empty2", empty2_solution, econ_pipes_dyn_prog(empty2));
         TEST_EQUAL("empty4", empty4_solution, econ_pipes_dyn_prog(empty4));
         TEST_EQUAL("horizontal", horizontal_solution, 
		    econ_pipes_dyn_prog(horizontal));
         TEST_EQUAL("vertical", vertical_solution, 
		    econ_pipes_dyn_prog(vertical));
         auto output = econ_pipes_dyn_prog(all_open);
         TEST_EQUAL("all_open path length", 7, output.steps().size());
         TEST_EQUAL("all_open total open cells", 6, output.total_open());
		   });

  rubric.criterion("dynamic programming - maze", 1,
		   [&]() {
         TEST_EQUAL("correct", maze_solution, econ_pipes_dyn_prog(maze));
		   });

  rubric.criterion("dynamic programming - random instances", 1,
		   [&]() {
         std::cout << std::endl;

         auto small_output = econ_pipes_dyn_prog(small_random);
         TEST_EQUAL("small", 4, small_output.total_open());
	 auto medium_output = econ_pipes_dyn_prog(medium_random);
         TEST_EQUAL("medium", 8, medium_output.total_open());
         auto large_output = econ_pipes_dyn_prog(large_random);
         TEST_EQUAL("large", 8, large_output.total_open());
		   });

  rubric.criterion("stress test", 2,
		   [&]() {
         const pipes::coordinate ROWS = 5,
                                  MAX_COLUMNS = 15;
         const unsigned SEED = 20181130;

         std::mt19937 gen(SEED);

         for (pipes::coordinate columns = 1; columns <= MAX_COLUMNS; 
	      ++columns) {
           auto area = ROWS * columns,
                open = area / 5,
                rocks = area / 10;
           pipes::grid setting = pipes::grid::random(ROWS, columns, open, 
						     rocks, gen);
           TEST_EQUAL("random grid with " + std::to_string(columns) + 
		      " columns",
                      pipes::econ_pipes_exhaustive(setting).total_open(),
                      pipes::econ_pipes_dyn_prog(setting).total_open());
         }
		   });

  return rubric.run();
}
