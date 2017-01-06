//
// Created by Louis GARCZYNSKI on 3/4/16.
//

#ifndef _HEURISTICS_H
# define _HEURISTICS_H

#include <string>
#include "Types.hpp"
#include "State.hpp"
#include <algorithm>

#define MAX_SOLUTION_LENGTH ((int)10000)
static const int score_multiplier = 4;

using Buffer = std::array<std::array<uchar, max_uid>, max_uid>;

namespace Heuristics
{
    constexpr Score				HeuristicFunction(const Data& data);
	constexpr Score				ValidFunction(const Data& data);
	constexpr Score				InvalidFunction(const Data& data);
    Score       				DatabaseFunction(const ID&);

	constexpr int		        SquareDistance(int uid_a, int uid_b);
    constexpr int		        SquareDistance(Coord a, Coord b);
    constexpr Buffer		    get_dist_table();
    const Buffer			    dist_table = get_dist_table();

};

#include "Heuristics.cpp"

#endif
