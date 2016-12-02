//
// Created by Louis GARCZYNSKI on 3/4/16.
//

#pragma once

#include <string>
#include "Types.hpp"
#include "State.hpp"

namespace Heuristics
{
    static Score	Distance(const Data& data);
    static int  	SquareDistance(Coord a, Coord b);

    using Buffer = std::array<std::array<std::array<std::array<std::array<std::array<char, 3>, 3>, 6>, 3>, 3>, 6>;

    static constexpr Buffer get_dist_table();
    static Buffer dist_table;

	static weighter	HeuristicFunction;
};
