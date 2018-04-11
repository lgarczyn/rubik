/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Heuristics.hpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lgarczyn <lgarczyn@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/04/09 21:35:33 by lgarczyn          #+#    #+#             */
/*   Updated: 2018/04/11 20:15:08 by lgarczyn         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include "State.hpp"
#include "Types.hpp"
#include <algorithm>
#include <string>

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