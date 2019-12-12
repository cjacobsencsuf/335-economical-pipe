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

  //1. maxlen = r + c - 2
  const size_t maxlen = setting.rows() + setting.columns() - 2;

  //2. best = None
  std::vector<std::vector<cell_type>> best(setting.rows(),
                               std::vector<cell_type>(setting.columns()));
  

  //check if never crossed X cell
  bool never_crossed_x = true;//start at true

  int bit;

  //check for best hasn't been filled yet
  bool best_still_empty = true;

  //current coordinates
  int current_row = 0;
  int current_col = 0;

  // compute each candidate and compare it with best
  //3. for bits from 0 to (2^maxlen - 1) inclusive
  for (int bits = 0; bits <= ((int)pow(2,maxlen) - 1); ++bits)
	{//for bits
	//4. candidate = [start]
	std::vector<std::vector<cell_type>> candidate(setting.rows(),
                               std::vector<cell_type>(setting.columns()));
	candidate[0][0] = path(setting);
  	assert(candidate[0][0].has_value());
	
	//5. for k from 0 to len - 1 inclusive:
	for(int k = 0; k <= total_steps; ++k)
		{//for k
		//6. bit = (bits >> k) & 1
		bit = (bits >> k) & 1;
		
		//7. if bit == 1
		if (bit == 1)
			{//if bit = 1
			//8. candidate.add(->)
			candidate.add_step(STEP_DIRECTION_RIGHT);
			++current_col;
			}//if bit = 1
		//9. else
		else
			{//else
			//10. candidate.add(V)
			candidate.add_step(STEP_DIRECTION_DOWN);
			++current_row;
			}//else

		//check for crossing x
		if(candidate.get(current_row, current_col) == CELL_ROCK)
			{//if cross x
			never_crossed_x = false;
			}//if cross x
		//11. if candidate stays inside the grid and never crosses an X cell
		if((candidate.is_row_column(current_row, current_col))&&(never_crossed_x))
			{//if no X
			//12. if best is None or candidate harvests more open cells than best:
				if((best_still_empty)||(candidate.total_open() > best.total_open()))
					{//if new best
					//13. best = candidate
					//store candidate in best
					best = candidate;
					best_still_empty = false;
					}//if new best
			}//if no X
		}//for k
	}//for bits
    
  //if (valid && (candidate.total_open() > best.total_open())) {
  //  best = candidate;
  // }
  //14. return best
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

  //1. A = new rxc matrix
  std::vector<std::vector<cell_type>> A(setting.rows(),
                               std::vector<cell_type>(setting.columns()));

  //2. base case
  //3. A[0][0] = start
  A[0][0] = path(setting);
  assert(A[0][0].has_value());

  //4. general cases
  //5. for i from 0 to r-1 inclusive
  for (coordinate r = 0; r < setting.rows(); ++r) 
	{//for r
	//6. for j from 0 to c-1 inclusive
    	for (coordinate c = 0; c < setting.columns(); ++c) 
		{//for c
			//7. if C[i][j] == X
			if (setting.get(r, c) == CELL_ROCK)
				{//if
				//8. A[i][j] = None
				A[r][c] = ;//TODO determine what the equivalent of None is
				//9. continue
				continue;
				}//if
      			if (setting.get(r, c) != CELL_ROCK) 
				{//if

        			auto best = A[r][c];

				// complete lines for computing from_above and from_left

				//10. from_above = from_left = None
				//TODO determine what an equivalent statement would be

				//11. if i > 0 and A[i-1][j] is not None
				if((r > 0)&&(A[r-1][c] != ))//TODO determine what the equivalent of None is
					{//if
					//12. from_above = A[i-1][j] + [down]
					 = A[r-1][c] + ;//TODO fill in the rest of the statement
					}//if

				//13. if j > 0 and A[i][j-1] is not None
				if((c > 0)&&(A[r][c-1] != ))//TODO determine what the equivalent of None is
					{//if
					//14. from_left = A[i][j-1] + [->]
					= A[r][c-1] + ;//TODO fill in the rest of the statement
					}//if

				//15. A[i][j] = whichever of from_above and from_left is non-None and harvests more open cells;
				//15. or None if both from_above and from_left are None
	
				//16. if from_above and from_left are None
				if(()&&())//TODO determine equivalent statement
					{//if
					//17. A[i][j] = None
					A[r][c] = ;//TODO determine what the equivalent of None is
					}//if
	
				//18. else if from_above is not None and harvests more open cells
				else if (()&&())//TODO determine equivalent statement
					{//else if
					//19. A[i][j] = from_above
					A[r][c] = ;//TODO determine what the equivalent of from_above is
					}//else if
				
				//20. else if from_left is not None and harvests more open cells
				else if (()&&())//TODO determine equivalent statement
					{//else if
					//21. A[i][j] = from_left
					A[r][c] = ;//TODO determine what the equivalent of from_left is
					}//else if
				
        			// then set A[r][c] = best;
				A[r][c] = best;
      				}//if
    		}//for c
  	}//for r
  
  //22. post-processing to find maximum open-cells path
  coordinate r = setting.rows()-1;
  coordinate c = setting.columns()-1;
  //23. best = A[r-1][c-1]
  cell_type * best = &(A[r][c]);
  assert(best->has_value());
  //24. return best
  return **best;
}//function
  
}
