//
// Created by Louis GARCZYNSKI on 3/4/16.
//

#pragma once

#include <string>
#include "Types.hpp"
#include "State.hpp"

class Heuristics
{
public:
    static Score	Distance(const Data& data, const Finder& finder);
    static int  	SquareDistance(Coord a, Coord b);

	static weighter	HeuristicFunction;
};
