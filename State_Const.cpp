//
// Created by Louis GARCZYNSKI on 1/7/17.
//

#include "State.hpp"

const Data	        State::solution_data = State::data_from_id(ID());
const Cube	        State::solution_cube = State::cube_from_id(ID());
const Finder	    State::solution_finder = State::_calculate_finder(solution_cube);
const Color	        State::solution_colors[] = {White, Green, Red, Blue, Orange, Yellow};