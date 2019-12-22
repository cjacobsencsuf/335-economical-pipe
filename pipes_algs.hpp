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

using namespace std;

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

  //1. maxlen = r + c - 2
  const size_t maxlen = setting.rows() + setting.columns() - 2;

  //2. best = None
  path best(setting);

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
	path candidate(setting);

	//5. for k from 0 to len - 1 inclusive:
	for(int k = 0; k <= total_steps; ++k)
		{//for k
		//6. bit = (bits >> k) & 1
		bit = (bits >> k) & 1;

		//7. if bit == 1
		if (bit == 1)
			{//if bit = 1
			//8. candidate.add(->)
			//check if possible
			if (candidate.is_step_valid(STEP_DIRECTION_RIGHT))
				{//if step right is valid
				candidate.add_step(STEP_DIRECTION_RIGHT);
				++current_col;
				}//if step right is valid
			}//if bit = 1
		//9. else
		else
			{//else
			//10. candidate.add(V)
			//check if possible
			if (candidate.is_step_valid(STEP_DIRECTION_DOWN))
				{//if step down is valid
				candidate.add_step(STEP_DIRECTION_DOWN);
				++current_row;
				}//if step down is valid
			}//else

		//11. if candidate stays inside the grid and never crosses an X cell
		//12. if best is None or candidate harvests more open cells than best:
		if((best_still_empty)||(candidate.total_open() > best.total_open()))
			{//if new best
			//13. best = candidate
			//store candidate in best
			best = candidate;
			best_still_empty = false;
			}//if new best
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

  using cell_type = std::optional<path>;

  //1. A = new rxc matrix
  std::vector<std::vector<cell_type>> A(setting.rows(),
                               std::vector<cell_type>(setting.columns()));

  //2. base case
  //3. A[0][0] = start
  A[0][0] = path(setting);
  assert(A[0][0].has_value());

  //create from_above and from_left
  path from_left(setting);
  path from_above(setting);


  //4. general cases
  //5. for i from 0 to r-1 inclusive
  for (coordinate r = 0; r < setting.rows(); ++r)
	{//for r
    //cout << "r = " << r << " / " << setting.rows() << endl;
	//6. for j from 0 to c-1 inclusive
    	for (coordinate c = 0; c < setting.columns(); ++c)
		{//for c
      //cout << "c = " << c << " / " << setting.columns() << endl;
			//7. if C[i][j] == X
			if (setting.get(r, c) == CELL_ROCK)
				{//if
				//8. A[i][j] = None
				//None is a value that shouldn't be there
				//choose a value that shouldn't be there when you need to put a None value
      //  cout << "cell rock hit" << endl;
				//9. continue
				continue;
				}//if
            else
				{//else
        //  cout << "no cell rock" << endl;
        			auto best = A[r][c];
          //    cout << "set best = A[r][c]" << endl;


				//11. if i > 0 and A[i-1][j] is not None
        if((r > 0) && A[r-1][c].has_value())
					{//if
					//12. from_above = A[i-1][j] + [down]
          //cout << "from above" << endl;
          from_above = A[r-1][c].value();
                //cout << "r = " << r << " c = " << c << endl;
                //cout << "add step down" << endl;
                from_above.add_step(STEP_DIRECTION_DOWN);
                //from_above.print();
              best = from_above;
					}//if

				//13. if j > 0 and A[i][j-1] is not None
        if((c > 0) && A[r][c-1].has_value())
					{//if
					//14. from_left = A[i][j-1] + [->]
          //cout << "from left" << endl;
          from_left = A[r][c-1].value();
              //cout << "r = " << r << " c = " << c << endl;
              //cout << "add step right" << endl;
              from_left.add_step(STEP_DIRECTION_RIGHT);
            //  from_left.print();

        if(!best.has_value() || from_left.total_open() > best.value().total_open())
            {
              best = from_left;
            }
					}//if
          A[r][c] = best;
        }//else
    		}//for c
  	}//for r
//cout << "done with loops" << endl;
  //22. post-processing to find maximum open-cells path
  coordinate r = setting.rows()-1;
  coordinate c = setting.columns()-1;
  //23. best = A[r-1][c-1]
//  cell_type * best = &(A[r][c]);
  //assert(best->has_value());
  //check for the best path

  for (coordinate i = r; i >= 0; --i)
    {
      for(coordinate j = c; j >= 0; --j)
        {
          //check for valid
          if(A[i][j].has_value())
            {
              cell_type * best = &(A[i][j]);
              assert(best->has_value());
              return **best;
            }//if
        }//for
    }//for

  //24. return best
  //return **best;
}//function
  
}
