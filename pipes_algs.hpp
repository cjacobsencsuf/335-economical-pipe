///////////////////////////////////////////////////////////////////////////////
// pipes_algs.hpp
//
// Algorithms that solve the ecomomical pipes problem.
//
// All of the TODO sections for this project reside in this file.
//
// This file builds on pipes_types.hpp, so you should familiarize yourself
// with that file before working on this file.
//
///////////////////////////////////////////////////////////////////////////////

#pragma once

#include <cassert>

#include "pipes_types.hpp"

namespace pipes {

// Solve the economical pipes problem for the given grid, using an exhaustive
// search algorithm.
//
// This algorithm is expected to run in exponential time, so the grid's
// width+height must be small enough to fit in a 64-bit int; this is enforced
// with an assertion.
//
// The grid must be non-empty.
path econ_pipes_exhaustive(const grid& setting) {

  // grid must be non-empty.
  assert(setting.rows() > 0);
  assert(setting.columns() > 0);

  // Compute the path length, and check that it is legal.
  const size_t total_steps = setting.rows() + setting.columns() - 2;
  assert(total_steps < 64);

  // TODO: implement the exhaustive search algorithm, then delete this
  // comment.

  path best(setting);

  // compute each candidate and compare it with best
    
  //if (valid && (candidate.total_open() > best.total_open())) {
  //  best = candidate;
  // }
  
  return best;
}
  
// Solve the economical pipes problem for the given grid, using a dynamic
// programming algorithm.
//
// The grid must be non-empty.
path econ_pipes_dyn_prog(const grid& setting) {

  // grid must be non-empty.
  assert(setting.rows() > 0);
  assert(setting.columns() > 0);

  // TODO: implement the dynamic programming algorithm, then delete this
  // comment.

  using cell_type = std::optional<path>;

  std::vector<std::vector<cell_type>> A(setting.rows(),
                               std::vector<cell_type>(setting.columns()));

  A[0][0] = path(setting);
  assert(A[0][0].has_value());

  for (coordinate r = 0; r < setting.rows(); ++r) {
    for (coordinate c = 0; c < setting.columns(); ++c) {
      if (setting.get(r, c) != CELL_ROCK) {

        auto best = A[r][c];

	// complete lines for computing from_above and from_left

        // then set A[r][c] = best;
      }
    }
  }
  
  coordinate r = setting.rows()-1;
  coordinate c = setting.columns()-1;
  cell_type * best = &(A[r][c]);
  assert(best->has_value());
  return **best;
}
  
}
