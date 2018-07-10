/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Heuristics.hpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lgarczyn <lgarczyn@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/04/09 21:35:33 by lgarczyn          #+#    #+#             */
/*   Updated: 2018/04/13 21:18:46 by lgarczyn         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include "State.hpp"
#include "Types.hpp"
#include <algorithm>
#include <string>

using Buffer = std::array<std::array<uchar, max_uid>, max_uid>;

namespace Heuristics {
	constexpr Score ValidFunction(const Data &data);
	constexpr Score CornerFunction(const Data &data);
	constexpr Score BorderFunction(const Data &data);
	Score DatabaseFunction(const ID &);
	Score DatabaseFunction(const IDG1 &);
	Score DatabaseFunction(const IDG2 &);

	constexpr int SquareDistance(int uid_a, int uid_b);
	constexpr int SquareDistance(Coord a, Coord b);
	constexpr Buffer get_dist_table();
	const Buffer dist_table = get_dist_table();
};