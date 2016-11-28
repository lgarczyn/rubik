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
    static score	Distance(const Data& data);

	static weighter	HeuristicFunction;
};
