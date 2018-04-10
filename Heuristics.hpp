/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Heuristics.hpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lgarczyn <lgarczyn@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/04/09 21:35:33 by lgarczyn          #+#    #+#             */
/*   Updated: 2018/04/09 22:36:46 by lgarczyn         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include <algorithm>
#include <string>
#include "State.hpp"
#include "Types.hpp"

#define MAX_SOLUTION_LENGTH ((int)10000)
static const int score_multiplier = 4;

using Buffer = std::array<std::array<uchar, max_uid>, max_uid>;

namespace Heuristics {
    constexpr Score HeuristicFunction(const Data &data);
    constexpr Score ValidFunction(const Data &data);
    constexpr Score InvalidFunction(const Data &data);
    Score DatabaseFunction(const ID &);

    constexpr int SquareDistance(int uid_a, int uid_b);
    constexpr int SquareDistance(Coord a, Coord b);
    constexpr Buffer get_dist_table();
    const Buffer dist_table = get_dist_table();
};

#include "Heuristics.cpp"